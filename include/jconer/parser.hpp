#ifndef __JCONER_PARSER_HPP__
#define __JCONER_PARSER_HPP__

#include "jconer/token.hpp"
#include "jconer/value.hpp"
#include "jconer/stream.hpp"
#include "jconer/error.hpp"
#include "common/logging.hpp"

namespace JCONER {

class Parser {
    CLASS_MAKE_LOGGER
    public:
        Parser();
        Parser(IStream& instream);
        
        JValue* parse();
        JValue* parse(IStream& instream);
        JValue* parseFromFile(std::string filename);
        inline PError error() const { return _err;}
    private:
        IStream* _instream;
        Token _cur_token;
        PError _err;
 
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

JValue* load(std::string filename, PError& err);
JValue* load(const char* filename, PError& err);
JValue* loads(const char* buffer, PError& err);
JValue* loads(std::string buffer, PError& err);

}

#endif
