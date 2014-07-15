#ifndef __JCONER_TOKEN_HPP__
#define __JCONER_TOKEN_HPP__

#include <string>
#include "logging.hpp"

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

    TT_END, // end of file

    TT_INVALID
};

class Token {
    public:
        Token(TokenType type)
            :_type(type), _lineno(-1), _col(-1)
        {
        }
        Token(TokenType type, int lineno, int col, std::string text)
            : _type(type), _lineno(lineno), _col(col), _text(text)
        {
            //_logger.setLevel(DEBUG);
            LOG_DEBUG("Generate a new token[%s]\n", toString().c_str());
        }
        Token(TokenType type, int lineno, int col, char c)
            : _type(type), _lineno(lineno), _col(col), _text(1, c)
        {
            //_logger.setLevel(DEBUG);
            LOG_DEBUG("Generate a new token[%s]\n", toString().c_str());
        }

        Token(const Token& t)
            : _type(t._type), _lineno(t._lineno), _col(t._col), _text(t._text)
        {
        }

        inline TokenType type() const { return _type; }
        static TokenType getTokenOne(char c);
        std::string toString();
        inline std::string text() const { return _text; }
    private:
        TokenType _type;
        int _lineno;
        int _col;
        std::string _text;

    CLASS_MAKE_LOGGER

};


}
#endif
