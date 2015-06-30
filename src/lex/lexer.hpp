#ifndef _LEXER_HPP
#define _LEXER_HPP

#include "io/input.hpp"
#include "token.hpp"

class Lexer {
    Input *input;

    public:
    Lexer(Input *_input);
    Token lex();
	Token lexRaw(); // does not throw away whitespace, comment tokens
	bool eof();

    protected:
    SourceLocation getLocation();
    String consumeWord();
    unsigned long long consumeDecSeq();
    unsigned long long consumeHexSeq();
    unsigned long long consumeOctSeq();
    unsigned long long consumeBinSeq();
    Token lexWord();
    Token lexNumericLiteral();
    Token lexStringLiteral();
    Token lexCharLiteral();
	Token lexPunctuator();
};

#endif
