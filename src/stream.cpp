#include "stream.hpp"
#include <fstream>
using namespace std;

namespace JCONER {

FileIStream::FileIStream(std::string filename)
    :_filename(filename)
{
    _fin.open(_filename.c_str());
    if (_fin) {
        LOG_ERROR("File doen't exist! [%s]", filename.c_str());        
    }
    _lineno = 0;
    _col = 0;
    _end = _buff = NULL;
    _cur_pos = 0;

    _file_length = -1;

    if ((_buff = malloc(BUFSIZE) ) == NULL) {
        LOG_FATAL("Run out of memory");
    }
    _end = _cur = _buff;
    _readBuffer();
}

void FileIStream::_readBuffer() {
    int remaining_length = _getRemainingLength();
    int length = BUFSIZE - 1 > remaining_length ? BUFSIZE : remaining_length + 1;
    _fin.read(_buff, length);
    _cur = _buff;
    _end = _buff + length;
}

inline int FileIStream::_getLength() {
    if (_file_length != -1)
        return _file_length;

    _cur_pos = _fin.tellg();
    _fin.seekg(0, ios::end);
    _file_length = _fin.tellg();

    _fin.seekg(_cur_pos, iso::beg);
    return _file_length;
}

inline int FileIStream::_getRemainingLength() {
    _cur_pos = _fin.tellg();
    return _getLength() - _cur_pos;
}

inline int FileIStream::_getNextChar() {
    if (_cur == _end) {
        _readBuffer();
    }
    return *_cur ++;
}

inline int FileIStream::_ungetChar() {
    if (_cur == _buff) return;
    _cur --;
}

void FileIStream::_stripWhitespace() {
    while(true) {
        int c = _getNextChar();
        if (isspace(c)) {
            if (c == '\n') {
                _lineno ++;
                _col = 0;
            } else {
                _col ++;
            }
        } else {
            return;
        }
    }
}

Token FileIStream::getNextToken() {
    _stripWhitespace();
    int c = _getNextChar();
    TokenType type;

    type = Token::getTokenOne(c);
    if (type != TT_INVALID) {
        Token t(type, _lineno, _col, (char)c);
        _col ++;
        return t;
    }
    
    if (c == '"') {
        VarString vs;
        while(true) { 
            c = _getNextChar();
            if (c == '\\') {
                int nextc = _getNextChar();
                switch(nextc) {
                    case '"':
                        vs.append('"');
                        break;
                    case '\\':
                        vs.append('\\');
                        break;
                    case '/':
                        vs.append('/');
                        break;
                    case 'b':
                        vs.append('\b');
                        break;
                    case 'f':
                        vs.append('\f');
                        break;
                    case 'n':
                        vs.append('\n');
                        break;
                    case 'r':
                        vs.append('\r');
                        break;
                    case 't':
                        vs.append('\t');
                        break;
                    case 'u':
                        vs.append("hex");
                        break;
                    default:
                        LOG_WARN("Wrong escape character");

                }
            }
            else if (c == '"') {
                Token t(TT_STRING, _lineno, _col, vs.toString());
                _col += vs.size();
                LOG_DEUG("Get a new string token[%s]\n", t.toString().c_str());
                return t;
            } else {
                vs.append((char)c);
            }
        }
    }
    
    if (c == '-' || isdigit(c)) {
        VarString vs;
        int nextc = 0;

        if (c == '-') {
            vs.append(c);
            c = _getNextChar();
        }

        vs.append(c);
        nextc = _getNextChar();

        if (isdigit(nextc)) {
            if (c == '0') {
                LOG_ERROR("NUMBER can't start with 0");
            }
            else {
                vs.append((char)nextc);
                while(true) {
                    c = _getNextChar();
                    if ( isdigit(c) ) {
                        vs.append((char)c);
                    } else {
                        break;
                    }
                }

                if (c == '.') {
frac:
                    vs.append((char)c); 
                    nextc = _getNextChar();
                    if (!isdigit(nextc)) {
                        LOG_ERROR("Unfinished fraction in number");
                    }
                    vs.append((char)nextc);
                    while(true) {
                        c = _getNextChar();
                        if (isdigit(c)) {
                            vs.append((char)c);
                        } else {
                            break;
                        }
                    }

                    if (c == 'e' || c == 'E') {
                        goto exp;
                    } else {
                        _ungetChar();
                        Token t(TT_REAL, _line, _col, vs.toString()); 
                        _col += vs.size();
                        return t;
                    }
                } else if (c == 'e' || c == 'E') {
exp:
                    vs.append((char)c);
                    nextc = _getNextChar(); 
                    if (nextc == '+' || nextc == '-') {
                        vs.append((char)c);
                    } else if (isdigit(nextc)) {
                        vs.append((char)nextc);
                        while(true) {
                            c = _getNextChar();
                            if (isdigit(c)) {
                                vs.append((char)c);
                            } else {
                                break;
                            }
                        }
                        _ungetChar();
                        Token t(TT_REAL, _lineno, _col, vs.toString());
                        _col += vs.size();
                        return t;
                    } else {
                        LOG_ERROR("Unfinished exp!");
                    }
                } else {
                    _ungetChar();
                    Token t(TT_INTEGER, _lineno, _col, vs.toString());
                    _col += vs.size();
                    return t;
                }
            }
        } else if (c == '.') {
            goto frac;
        } else if (c == 'e' || c == 'E') {
            goto exp;
        } else {
            _ungetChar();
            Token t(TT_INTEGER, _lineno, _col, vs.toString());
            _col += vs.size();
            return t;
        }

    }

    if (c == 't' || c == 'f' || c == 'n') { // should be true, false or null
        VarString vs;
        vs.append(c);
        for(int i = 0; i < 3; i ++ ) {
           c = _getNextChar();
           if (c == EOF) {
               LOG_ERROR("End of File");
           } else {
               va.append((char)c);
           }
        }

        if (strncmp(va.toString().c_str(), "true", 4) == 0) {
            Token t(TT_TRUE, _lineno, _col, vs.toString());
            _col += vs.size();
            return t;
        } else if (strncmp(va.toString().c_str(), "null", 4) == 0) {
            Token t(TT_NULL, _lineno, _col, vs.toString());
            _col += vs.size();
            return t;
        } else if (strncmp(va.toString().c_str(), "false", 4) == 0) {
            c = _getNextChar();
            if (c == 'e') {
                vs.append((char)c);
                Token t(TT_FALSE, _lineno, _col, vs.toString());
                _col += vs.size();
                return t;
            } else {
                LOG_ERROR("Unknown token");
            }
        }
    } else { // invalid
        return Token(TT_INVALID, _lineno, _col, "");
    }
}

}
