#ifndef __JCONER_TOKEN_HPP__
#define __JCONER_TOKEN_HPP__

namespace JCONER {

enum TokenType {
    TT_ARRAY_OPEN_BRACE = 0,
    TT_ARRAY_CLOSE_BRACE,
    TT_OBJECT_OPEN_BRACE,
    TT_OBJECT_CLOSE_BRACE,

    TT_COMMA,
    TT_COLON,

    TT_STRING,
    TT_INTEGER,
    TT_REAL,

    TT_TRUE,
    TT_FALSE,

    TT_NULL;
};

class Token {
    public:
        Token(TokenType type, int lineno, int col, string text);
};


}
#endif
