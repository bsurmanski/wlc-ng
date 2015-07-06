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
    unsigned long long consumeDecSeq(int *len = NULL);
    unsigned long long consumeHexSeq(int *len = NULL);
    unsigned long long consumeOctSeq(int *len = NULL);
    unsigned long long consumeBinSeq(int *len = NULL);
    Token lexWord();
    Token lexNumericLiteral();
    Token lexStringLiteral();
	Token lexRawStringLiteral();
    Token lexCharLiteral();
	Token lexPunctuator();
	Token lexBlockComment();
	Token lexLineComment();
};

#endif
