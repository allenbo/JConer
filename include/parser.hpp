#ifndef __JCONER_PARSER_HPP__
#define __JCONER_PARSER_HPP__

#include "token.hpp"
#include "value.hpp"
#include "stream.hpp"
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

JValue* load(std::string filename);
JValue* load(const char* filename);
//JValue* loads(const char* buffer);

}

#endif
