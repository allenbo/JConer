#ifndef __JCONER_STREAM_HPP__
#define __JCONER_STREAM_HPP__

#include "jconer/token.hpp"
#include "common/logging.hpp"
#include "jconer/error.hpp"

#include <istream>
#include <fstream>


namespace JCONER {

class IStream {
    public:
        IStream(std::istream& _fin);
        virtual Token getNextToken();
        inline PError error() const { return _err; }
        virtual ~IStream();
    private:
        int _getLength();
        int _getRemainingLength();

        int _getNextChar();
        void _ungetChar();
        bool _stripWhitespace();
        int _readBuffer();
        int _getNChar(char* str, int n);

        std::istream& _fin;
        int _lineno;
        int _col;
        char* _buff;
        char* _end;
        char* _cur;
        int _cur_pos;
        int _content_length;
        PError _err;
};

}
#endif
