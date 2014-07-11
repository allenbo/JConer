#ifndef __JCONER_STREAM_HPP__
#define __JCONER_STREAM_HPP__

#include "token.hpp"
#include "logging.hpp"

#include <fstream>

namespace JCONER {

enum StreamType {
    ST_FILE = 0,
    ST_BUFFER
};

class IStream {
    public:
        IStream();
        virtual Token getNextToken() = 0;
        virtual ~IStream();

    protected:
        enum StreamType _type;
};


class FileIStream {
    public:
        FileIStream(std::string filename);
        virtual Token getNextToken();
        virtual ~FileIStream();
    private:
        std::string _filename;
        std::ifstream _fin;

    CLASS_MAKE_LOGGER
};

class BufferIStream {
    public:
        BufferIStream(std::string buffer);
        BufferIStream(char* buffer, int n);
        virtual Token getNextToken();
        virtual ~BufferIStream();


    CLASS_MAKE_LOGGER

};

}
#endif
