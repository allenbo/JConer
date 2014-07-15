#ifndef __JCONER_STREAM_HPP__
#define __JCONER_STREAM_HPP__

#include "token.hpp"
#include "logging.hpp"
#include "util.hpp"

#include <fstream>


namespace JCONER {

enum StreamType {
    ST_FILE = 0,
    ST_BUFFER
};

class IStream {
    public:
        IStream() {};
        virtual Token getNextToken() = 0;
        virtual ~IStream() {};

    protected:
        enum StreamType _type;
};


class FileIStream : public IStream {
    public:
        FileIStream(std::string filename);
        virtual Token getNextToken();
        virtual ~FileIStream();
    private:
        int _getLength();
        int _getRemainingLength();

        int _getNextChar();
        void _ungetChar();
        bool _stripWhitespace();
        int _readBuffer();
        int _getNChar(char* str, int n);

        std::string _filename;
        std::ifstream _fin;
        int _lineno;
        int _col;
        char* _buff;
        char* _end;
        char* _cur;
        int _cur_pos;
        int _file_length;

    CLASS_MAKE_LOGGER
};
/*
class BufferIStream {
    public:
        BufferIStream(std::string buffer);
        BufferIStream(char* buffer, int n);
        virtual Token getNextToken();
        virtual ~BufferIStream();


    CLASS_MAKE_LOGGER

};
*/
}
#endif
