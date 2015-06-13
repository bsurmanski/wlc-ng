#include "lexer.hpp"
#include "exception/exception.hpp"

#include <assert.h>

static char lowercase(char c) {
    if(c >= 'A' && c <= 'Z') {
        return c - 'A' + 'a';
    }
    return c;
}

static bool isNonDigit(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

static bool isDigit(char c) {
    return c >= '0' && c <= '9';
}

static bool isIdChar(char c) {
    return isNonDigit(c) || isDigit(c);
}

static bool isSign(char c) {
    return c == '-' || c == '+';
}

static bool isBinDigit(char c) {
    return c == '0' || c == '1';
}

static bool isOctDigit(char c) {
    return c >= '0' && c <= '7';
}

static bool isHexDigit(char c) {
    char low = lowercase(c);
    return (c >= '0' && c <= '9') || (low >= 'a' && low <= 'f');
}

Lexer::Lexer(Input *_input) {
    input = _input;
}

Token Lexer::lex() {
    char p = input->peek();
    if(isDigit(p)) {
        return lexNumericLiteral();
    }

    if(isNonDigit(p)) {
        return lexWord();
    }

    if(p == '"') {
        return lexStringLiteral();
    }

    if(p == '\'') {
        return lexCharLiteral();
    }

	throw new Exception("invalid input character found");
}

SourceLocation Lexer::getLocation() {
    return SourceLocation(input, input->tell());
}

String Lexer::consumeWord() {
    String str;

    if(!isNonDigit(input->peek())) return "";

    do {
        str += (char) input->get();
    } while (isIdChar(input->peek()) && !input->eof());
    return str;
}

Token Lexer::lexWord() {
    SourceLocation loc = getLocation();
    String word = consumeWord();

#define KEYWORD(X) if(word == #X) return Token(tok::kw_##X, loc);
#include "tokenkinds.def"
#undef KEYWORD

    return Token(tok::identifier, loc);
}

String Lexer::consumeDecSeq() {
    String str;
    while(isDigit(input->peek())) {
        str += (char) input->get();
    }
    return str;
}

String Lexer::consumeHexSeq() {
    String str;
    while(isHexDigit(input->peek())) {
        str += (char) input->get();
    }
    return str;
}

String Lexer::consumeOctSeq() {
    String str;
    while(isOctDigit(input->peek())) {
        str += (char) input->get();
    }
    return str;
}

String Lexer::consumeBinSeq() {
    String str;
    while(isBinDigit(input->peek())) {
        str += (char) input->get();
    }
    return str;
}

Token Lexer::lexNumericLiteral() {
	SourceLocation loc = getLocation();
    bool negative = false;

    if(isSign(input->peek())) {
        char s = input->get();
        if(s == '-') negative = true;
    }

    String num;
    num += (char) input->get();

    if(num[0] == '0') {
        char c = input->peek();
        if(c == 'x') {
            input->get();
            consumeHexSeq();
        } else if(c == 'o') {
            input->get();
            consumeOctSeq();
        } else if(c == 'b') {
            input->get();
            consumeBinSeq();
        }
    }
	
	return Token(tok::intlit, loc);
}

Token Lexer::lexStringLiteral() {
	SourceLocation loc = getLocation();
	return Token(tok::stringlit, loc);
}


Token Lexer::lexCharLiteral() {
    SourceLocation loc = getLocation();
    assert(input->peek() == '\'');
    input->get();

    if(input->peek() == '\\') {
        //TODO: handle escape char
    }

    char c = input->get();
    return Token(tok::charlit, loc); //TODO: set value
}
