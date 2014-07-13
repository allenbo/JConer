#include "token.hpp"

namespace JCONER {

TokenType Token::getTokenOne(char c) {
    switch(c) {
        case '[':
            return TT_ARRAY_OPEN_BRACE;
        case ']':
            return TT_ARRAY_CLOSE_BRACE;
        case '{':
            return TT_ARRAY_OPEN_BRACE;
        case '}':
            return TT_ARRAY_CLOSE_BRACE;
        case ',':
            return TT_COMMA;
        case ':':
            return TT_COLON;
        default:
            return TT_INVALID;
    }
}

std::string Token::toString() {
    return _text;
}
 
}
