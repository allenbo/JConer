#ifndef __JCONER_PARSER_HPP__
#define __JCONER_PARSER_HPP__

#include "value.hpp"
#include "stream.hpp"
#include "token.hpp"
#include "logging.hpp"

namespace JCONER {

class Parser {
    CLASS_MAKE_LOGGER
    public:
        Parser();
        Parser(IStream& instream);
        
        JValue* parse();
        JValue* parse(IStream& instream);
        JValue* parseFromFile(std::string filename);
    private:
        IStream* _instream;
        Token _cur_token;
 
        void _getNextToken();
        JValue* _parseValue();
        JValue* _parseInt();
        JValue* _parseString();
        JValue* _parseReal();
        JValue* _parseTrue();
        JValue* _parseFalse();
        JValue* _parseNull();
        JValue* _parseArray();
        JValue* _parseObject();

        inline bool _checkTokenType(TokenType type) const { return _cur_token.type() == type;}
};

}

#endif
