#include "token.hpp"

namespace JCONER {

static const char* TokenType_Str[] = {
    "array_open_brace",
    "array_close_brace",
    "object_open_brace",
    "object_close_brace",

    "comma",
    "colon",
    "string",
    "integer",
    "real",

    "true",
    "false",
    "null",

    "end",
    "invalid",
    NULL
};

TokenType Token::getTokenOne(char c) {
    switch(c) {
        case '[':
            return TT_ARRAY_OPEN_BRACE;
        case ']':
            return TT_ARRAY_CLOSE_BRACE;
        case '{':
            return TT_OBJECT_OPEN_BRACE;
        case '}':
            return TT_OBJECT_CLOSE_BRACE;
        case ',':
            return TT_COMMA;
        case ':':
            return TT_COLON;
        case EOF:
            return TT_END;
        default:
            return TT_INVALID;
    }
}

std::string Token::toString() {
    std::string rst(TokenType_Str[_type]);
    return rst + " " + _text;
}
 
}
