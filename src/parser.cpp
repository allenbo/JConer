#include "parser.hpp"
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include <errno.h>

namespace JCONER {

Parser::Parser()
    :_instream(NULL), _cur_token(TT_END)
{
}

Parser::Parser(IStream& instream)
    :_instream(&instream), _cur_token(TT_END)
{
}

JValue* Parser::parse() {
    _getNextToken();
    return _parseValue();
}

JValue* Parser::parse(IStream& instream) {
    _instream = &instream;
    _getNextToken();
    return _parseValue();
}

void Parser::_getNextToken() {
    if (_instream == NULL) {
        LOG_FATAL("There is no stream\n");
    }
    _cur_token = _instream->getNextToken(); 
}

JValue* Parser::_parseValue() {
    switch(_cur_token.type()) {
        case TT_STRING:
            return _parseString();
        case TT_INTEGER:
            return _parseInt();
        case TT_REAL:
            return _parseReal();
        case TT_TRUE:
            return _parseTrue();
        case TT_FALSE:
            return _parseFalse();
        case TT_NULL:
            return _parseNull();
        case TT_ARRAY_OPEN_BRACE:
            return _parseArray();
        case TT_OBJECT_OPEN_BRACE:
            return _parseObject();
        default:
            LOG_WARN("INVALID TOKEN[%s]\n", _cur_token.toString().c_str());
            return NULL;
    }
}

JValue* Parser::_parseInt() {
    long value = strtol(_cur_token.text().c_str(), NULL, 10);
    if (errno == ERANGE) {
        LOG_WARN("Value out of range[%s]\n", _cur_token.text().c_str());
    }
    return new JInt(value); 
}

JValue* Parser::_parseString() {
    return new JString(_cur_token.text());
}

JValue* Parser::_parseReal() {
    double value = strtod(_cur_token.text().c_str(), NULL);
    if (errno == ERANGE) {
        LOG_WARN("Value out of range[%s]\n", _cur_token.text().c_str());
    }
    return new JReal(value);
}

JValue* Parser::_parseTrue() {
    return new JTrue();
}

JValue* Parser::_parseFalse() {
    return new JFalse();
}

JValue* Parser::_parseNull() {
    return new JNull();
}

JValue* Parser::_parseArray() {
    _getNextToken();
    JValue* rst = new JArray();
    JValue* elt = NULL;
    
    if (_cur_token.type() == TT_ARRAY_CLOSE_BRACE) 
        return rst;

    while(true) {
        elt = _parseValue();
        ((JArray*)rst)->append(elt);
        _getNextToken();
        switch(_cur_token.type()) {
            case TT_COMMA:
                _getNextToken();
                break;
            case TT_ARRAY_CLOSE_BRACE:
                return rst;
            default:
                LOG_ERROR("Unfinished array!\n");
        }
    }   
    return rst;
}

JValue* Parser::_parseObject() {
    JValue* rst = new JObject();

    JValue* value = NULL;
    std::string key;

    _getNextToken();
    if(_cur_token.type() == TT_OBJECT_CLOSE_BRACE) {
        return rst;
    }

    while(true) {
        if (!_checkTokenType(TT_STRING) ) {
            LOG_ERROR("String token expected in object!\n");
        }
        key = _cur_token.text();

        _getNextToken();
        if (!_checkTokenType(TT_COLON)) {
            LOG_ERROR("Colon token expected in object!\n");
        }

        _getNextToken();
        value = _parseValue();
        ((JObject*)rst)->put(key, value);

        _getNextToken();
        switch(_cur_token.type()) {
            case TT_COMMA:
                _getNextToken();
                break;
            case TT_OBJECT_CLOSE_BRACE:
                return rst;
            default:
                LOG_ERROR("Unfinished array!\n");
        }
    }
    return rst;
}

}
