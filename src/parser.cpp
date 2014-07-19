#include "jconer/parser.hpp"
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include <errno.h>
#include <sstream>

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

JValue* Parser::parseFromFile(std::string filename) {
    std::ifstream in(filename);
    IStream fin(in);
    _instream = &fin;
    return parse();
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
            _err = _instream->error();
            return NULL;
    }
}

JValue* Parser::_parseInt() {
    long value = strtol(_cur_token.text().c_str(), NULL, 10);
    if (errno == ERANGE) {
        _err.setErrorDetail(_cur_token.lineno(), _cur_token.col(), ET_PARSE_RANGE, "Value ouf of range[%s]\n", _cur_token.text().c_str());
        return NULL;
    }
    return new JInt(value); 
}

JValue* Parser::_parseString() {
    return new JString(_cur_token.text());
}

JValue* Parser::_parseReal() {
    double value = strtod(_cur_token.text().c_str(), NULL);
    if (errno == ERANGE) {
        _err.setErrorDetail(_cur_token.lineno(), _cur_token.col(), ET_PARSE_RANGE, "Value ouf of range[%s]\n", _cur_token.text().c_str());
        return NULL;
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
                _err.setErrorDetail(_cur_token.lineno(),
                                    _cur_token.col(),
                                    ET_PARSE_UNEXPECTED_TOKEN,
                                    "Unexpected token while parsing array\n",
                                    _cur_token.text().c_str());
                return NULL;
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
            goto fail;
        }
        key = _cur_token.text();

        _getNextToken();
        if (!_checkTokenType(TT_COLON)) {
            goto fail;
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
                goto fail;
        }
    }
    return rst;
fail:
    _err.setErrorDetail(_cur_token.lineno(),
                        _cur_token.col(),
                        ET_PARSE_UNEXPECTED_TOKEN,
                        "Unexpected token while parsing object\n",
                        _cur_token.text().c_str());
    return NULL;

}

JValue* load(std::string filename, PError& err) {
    std::ifstream in(filename);
    if (!in.good()) {
        err.type = ET_IO_FILE_NOT_FOUND;
        return NULL;
    }
    IStream fin(in);
    Parser parser(fin);
    JValue* rst = parser.parse();
    if (rst == NULL) {
        err = parser.error();
    }
    in.close();
    return rst;
}

JValue* load(const char* filename, PError& err) {
    std::string str_filename(filename);
    return load(str_filename, err);
}

JValue* loads(const char* buffer, PError& err) {
    std::string str_buffer(buffer);
    return loads(str_buffer, err);
}

JValue* loads(std::string buffer, PError& err) {
    std::stringstream ssin(buffer);
    IStream fin(ssin);
    Parser parser(fin);
    JValue* rst = parser.parse();
    if (rst == NULL) {
        err = parser.error();
    }
    return rst;
}

}
