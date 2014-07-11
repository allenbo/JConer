#ifndef __JCONER_STREAM_HPP__
#define __JCONER_STREAM_HPP__

#include "token.hpp"

namespace JCONER {

enum StreamType {
    ST_FILE = 0,
    ST_BUFFER
};

class IStream {
    public:
        IStream();
        virtual Token getNextToken();
        virtual ~IStream() = 0;

    protected:
        enum StreamType _type;
};


class FileIStream {
    public:
        FileIStream(string filename);
        virtual Token getNextToken();
        virtual ~FileIStream();
};

class BufferIStream {
    public:
        BufferIStream(string buffer);
        BufferIStream(char* buffer, int n);
        virtual Token getNextToken();
        virtual ~BufferIStream();

};

}
#endif
