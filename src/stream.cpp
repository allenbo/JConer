#include "stream.hpp"
#include <fstream>
using namespace std;

namespace JCONER {

FileIStream::FileIStream(std::string filename)
    :_filename(filename)
{
    _fin.open(_filename.c_str());
    if (!_fin) {
        LOG_ERROR("File doen't exist! [%s]\n", _filename.c_str());        
    }
    _lineno = 0;
    _col = 0;
    _end = _buff = NULL;
    _cur_pos = 0;

    _file_length = -1;

    if ((_buff = (char*) malloc(BUFSIZE) ) == NULL) {
        LOG_FATAL("Run out of memory");
    }
    _end = _cur = _buff;
    _readBuffer();
}

FileIStream::~FileIStream() {
    free(_buff);
}

int FileIStream::_readBuffer() {
    int remaining_length = _getRemainingLength();
    if (remaining_length == 0) {
        return EOF;
    }
    int length = BUFSIZE > remaining_length ? remaining_length : BUFSIZE;
    _fin.read(_buff, length);
    _cur = _buff;
    _end = _buff + length;
    return 0;
}

int FileIStream::_getNChar(char* str, int n) {
    if (n == 0) return 0;

    int i;
    int c = 0;
    for(i = 0; i < n; i ++) {
        c = _getNextChar();
        if (c == EOF) {
            break;
        }
        str[i] = (char)c;
    }
    return i;
}

inline int FileIStream::_getLength() {
    if (_file_length != -1)
        return _file_length;

    _cur_pos = _fin.tellg();
    _fin.seekg(0, ios::end);
    _file_length = _fin.tellg();

    _fin.seekg(_cur_pos, ios::beg);
    return _file_length;
}

inline int FileIStream::_getRemainingLength() {
    _cur_pos = _fin.tellg();
    return _getLength() - _cur_pos;
}

inline int FileIStream::_getNextChar() {
    if (_cur == _end) {
        int rst = _readBuffer();
        if (rst == EOF) {
            return EOF;
        }
    }
    return *_cur ++;
}

inline void FileIStream::_ungetChar() {
    if (_cur == _buff) return;
    _cur --;
}

bool FileIStream::_stripWhitespace() {
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
            if (c == EOF) return true;
            _ungetChar();
            return false;
        }
    }
}

Token FileIStream::getNextToken() {
    bool iseof = _stripWhitespace();
    if (iseof) { return Token(TT_END);}
    int c = _getNextChar();
    TokenType type;

    type = Token::getTokenOne(c);
    if (type != TT_INVALID) {
        Token t(type, _lineno, _col, (char)c);
        _col ++;
        return t;
    }

    if (c == '"') {
        int esc_count = 0;
        VarString vs;
        while(true) { 
            c = _getNextChar();
            if (c == '\\') {
                esc_count ++;
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
                        {
                        esc_count ++;
                        char tmp[4];
                        if (_getNChar(tmp, 4) != 4) {
                            LOG_ERROR("Unexpected end of file\n");
                        }
                        esc_count += 4;
                        int32_t value = HexCode::decode(tmp);

                        if (value < 0) {
                            LOG_ERROR("Invalid unicode escape\n");
                        }

                        if (0xD800 <= value && value <= 0xDBFF) {
                            c = _getNextChar();
                            nextc = _getNextChar();
                            if (c == '\\' && nextc == 'u') {
                                if (_getNChar(tmp, 4) != 4) {
                                    LOG_ERROR("Unexpected end of file\n");
                                }

                                int32_t value2 = HexCode::decode(tmp);
                                if (0xDC00 <= value2 && value2 <= 0xDFFF) {
                                    value = ((value - 0xD800) << 10) + (value2 - 0xDC00) + 0x10000;
                                    esc_count += 6;
                                } else {
                                    LOG_ERROR("Invalid unicode \\u%04X\\u%04X\n", value, value2);
                                }
                            } else {
                                _ungetChar();
                                _ungetChar();
                                LOG_ERROR("Invalid unicode \\u%04X\n", value);
                            }
                        } else if (0xDC00 <= value && value <= 0xDFFF) {
                            LOG_ERROR("Invalid unicode \\u%04X\n", value);
                        }
                        int utf8_size = UTF8::encode(value, tmp, 4);
                        if (utf8_size < 0) {
                            LOG_ERROR("Invalide encoding utf8 \\u%04X\n", value);
                        }

                        esc_count -= utf8_size;
                        vs.append(tmp, utf8_size);
                        break;
                        }
                    default:
                        LOG_WARN("Wrong escape character");

                }
            }
            else if (c == '"') {
                Token t(TT_STRING, _lineno, _col, vs.toString());
                _col += vs.size() + esc_count + 2/* For " and " */;
                LOG_DEBUG("Get a new string token[%s]\n", t.toString().c_str());
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
                        Token t(TT_REAL, _lineno, _col, vs.toString()); 
                        _col += vs.size();
                        return t;
                    }
                } else if (c == 'e' || c == 'E') {
exp:
                    vs.append((char)c);
                    nextc = _getNextChar(); 
                    if (nextc == '+' || nextc == '-') {
                        vs.append((char)nextc);
                    } else if (isdigit(nextc)) {
                        vs.append((char)nextc);
                    } else {
                        LOG_ERROR("Unfinished exp!");
                    }
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
                    _ungetChar();
                    Token t(TT_INTEGER, _lineno, _col, vs.toString());
                    _col += vs.size();
                    return t;
                }
            }
        } else if (nextc == '.') {
            c = nextc;
            goto frac;
        } else if (nextc == 'e' || nextc == 'E') {
            c = nextc;
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
                vs.append((char)c);
            }
        }

        if (strncmp(vs.toString().c_str(), "true", 4) == 0) {
            Token t(TT_TRUE, _lineno, _col, vs.toString());
            _col += vs.size();
            return t;
        } else if (strncmp(vs.toString().c_str(), "null", 4) == 0) {
            Token t(TT_NULL, _lineno, _col, vs.toString());
            _col += vs.size();
            return t;
        } else if (strncmp(vs.toString().c_str(), "false", 4) == 0) {
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
    }
    return Token(TT_INVALID, _lineno, _col, "");
}

}
