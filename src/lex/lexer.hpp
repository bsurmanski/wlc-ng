#ifndef _LEXER_HPP
#define _LEXER_HPP

#include "../io/input.hpp"
#include "token.hpp"

class Lexer {
    Input *input;

    public:
    Lexer(Input *_input);
    Token lex();

    protected:
    SourceLocation getLocation();
    std::string consumeWord();
    std::string consumeDecSeq();
    std::string consumeHexSeq();
    std::string consumeOctSeq();
    std::string consumeBinSeq();
    Token lexWord();
    Token lexNumericLiteral();
    Token lexStringLiteral();
    Token lexCharLiteral();
};

#endif
