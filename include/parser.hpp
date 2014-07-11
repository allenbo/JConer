#ifndef __JCONER_PARSER_HPP__
#define __JCONER_PARSER_HPP__

#include "value.hpp"
#include "stream.hpp"
#include "token.hpp"

namespace JCONER {

class Parser {
    public:
        Parse();
        Parser(IStream& instream);
        
        Value parse();
        Value parse(IStream& instream);
};

}

#endif
