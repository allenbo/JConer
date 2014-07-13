#ifndef __JCONER_TOKEN_HPP__
#define __JCONER_TOKEN_HPP__

#include <string>

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
    TT_NULL,

    TT_INVALIAD
};

class Token {
    public:
        Token(TokenType type, int lineno, int col, std::string text)
            : _type(type), _lineno(lineno), _col(col), _text(text) {}
 
        static TokenType getTokenOne(char c);
        std::string toString();
    private:
        TokenType _type;
        int _lineno;
        int _col;
        std::string _text;
};


}
#endif
