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
    _end = _start = _buff = NULL;
    _cur_pos = 0;

    _file_length = -1;

    if ((_buff = malloc(BUFSIZE) ) == NULL) {
        LOG_FATAL("Run out of memory");
    }
    _end = _start = _cur = _buff;
    _readBuffer();
}

void FileIStream::_readBuffer() {
    int remaining_length = _getRemainingLength();
    int length = BUFSIZE - 1 > remaining_length ? BUFSIZE : remaining_length + 1;
    _fin.read(_buff, length);
    _start = _buff;
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
    if (_start == _end) {
        _readBuffer();
    }
    return *_cur ++;
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
        }
    }
}

}
