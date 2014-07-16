#include "stream.hpp"
#include <fstream>
using namespace std;

namespace JCONER {

IStream::IStream(istream& in)
    :_fin(in)
{
    _lineno = 0;
    _col = 0;
    _end = _buff = NULL;
    _cur_pos = 0;

    _content_length = -1;

    if ((_buff = (char*) malloc(BUFSIZE) ) == NULL) {
        LOG_FATAL("Run out of memory");
    }
    _end = _cur = _buff;
    _readBuffer();
}

IStream::~IStream() {
    free(_buff);
}

int IStream::_readBuffer() {
    int remaining_length = _getRemainingLength();
    if (remaining_length == 0) {
        return EOF;
    }
    int length = BUFSIZE > remaining_length ? remaining_length : BUFSIZE;
    _fin.read(_buff, length);
    //TODO Error handling
    _cur = _buff;
    _end = _buff + length;
    return 0;
}

int IStream::_getNChar(char* str, int n) {
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

inline int IStream::_getLength() {
    if (_content_length != -1)
        return _content_length;

    _cur_pos = _fin.tellg();
    _fin.seekg(0, ios::end);
    _content_length = _fin.tellg();

    _fin.seekg(_cur_pos, ios::beg);
    return _content_length;
}

inline int IStream::_getRemainingLength() {
    _cur_pos = _fin.tellg();
    return _getLength() - _cur_pos;
}

inline int IStream::_getNextChar() {
    if (_cur == _end) {
        int rst = _readBuffer();
        if (rst == EOF) {
            return EOF;
        }
        //TODO Error handling
    }
    return *_cur ++;
}

inline void IStream::_ungetChar() {
    if (_cur == _buff) return;
    _cur --;
}

bool IStream::_stripWhitespace() {
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

Token IStream::getNextToken() {
    _err.clear();
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
                        char tmp[5];
                        if (_getNChar(tmp, 4) != 4) {
                            _err.setErrorDetail(_lineno, _col, ET_PARSE_EOF, "Unexpected end of file\n");
                            goto fail;
                        }
                        esc_count += 4;
                        int32_t value = HexCode::decode(tmp);

                        if (value < 0) {
                            tmp[4] = '\0';
                            _err.setErrorDetail(_lineno, _col, ET_PARSE_INVALID_UNICODE, "Invalid unicode \\u%s\n", tmp) ;
                            goto fail;
                        }

                        if (0xD800 <= value && value <= 0xDBFF) {
                            c = _getNextChar();
                            nextc = _getNextChar();
                            if (c == '\\' && nextc == 'u') {
                                if (_getNChar(tmp, 4) != 4) {
                                    _err.setErrorDetail(_lineno, _col, ET_PARSE_EOF, "Unexpected end of file\n");
                                    goto fail;
                                }

                                int32_t value2 = HexCode::decode(tmp);
                                if (0xDC00 <= value2 && value2 <= 0xDFFF) {
                                    value = ((value - 0xD800) << 10) + (value2 - 0xDC00) + 0x10000;
                                    esc_count += 6;
                                } else {
                                    _err.setErrorDetail(_lineno, _col, ET_PARSE_INVALID_UNICODE, "Invalid unicode \\u%04X\\u%04X\n", value, value2);
                                    goto fail;
                                }
                            } else {
                                _ungetChar();
                                _ungetChar();
                                _err.setErrorDetail(_lineno, _col, ET_PARSE_INVALID_UNICODE, "Invalid unicode \\u%04X\n", value);
                                goto fail;
                            }
                        } else if (0xDC00 <= value && value <= 0xDFFF) {
                            _err.setErrorDetail(_lineno, _col, ET_PARSE_INVALID_UNICODE, "Invalid unicode \\u%04X\n", value);
                            goto fail;
                        }
                        int utf8_size = UTF8::encode(value, tmp, 4);
                        if (utf8_size < 0) {
                            _err.setErrorDetail(_lineno, _col, ET_PARSE_INVALID_UNICODE, "Unicode \\u%04X can't convert to utf8\n", value);
                            goto fail;
                        }

                        esc_count -= utf8_size;
                        vs.append(tmp, utf8_size);
                        break;
                        }
                    default:
                        _err.setErrorDetail(_lineno, _col, ET_PARSE_INVALID_CHAR, "Wrong escape char %c\n", (char)c);
                        goto fail;
                }
            }
            else if (c == '"') {
                Token t(TT_STRING, _lineno, _col, vs.toString());
                _col += vs.size() + esc_count + 2/* For " and " */;
                return t;
            } else if (c == EOF) {
                _err.setErrorDetail(_lineno, _col, ET_PARSE_EOF, "Unexpected end of file\n");
                goto fail;
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
                _err.setErrorDetail(_lineno, _col, ET_PARSE_INVALID_CHAR, "Number can't start with 0\n");
                goto fail;
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
                        _err.setErrorDetail(_lineno, _col, ET_PARSE_INVALID_CHAR, "Unfinished fraction in number\n");
                        goto fail;
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
                        c = _getNextChar();
                        if (!isdigit(c)) {
                            _err.setErrorDetail(_lineno, _col, ET_PARSE_INVALID_CHAR, "Unfinished exp in number\n");
                            goto fail;
                        }
                    } else if (isdigit(nextc)) {
                        vs.append((char)nextc);
                    } else {
                        _err.setErrorDetail(_lineno, _col, ET_PARSE_INVALID_CHAR, "Unfinished exp in number\n");
                        goto fail;
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
                _err.setErrorDetail(_lineno, _col, ET_PARSE_EOF, "Unexpected enf of file\n");
                goto fail;
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
                _err.setErrorDetail(_lineno, _col, ET_PARSE_INVALID_CHAR, "Unknown token\n");
                goto fail;
            }
        }
    }
fail:
    return Token(TT_INVALID, _lineno, _col, "");
}

}
