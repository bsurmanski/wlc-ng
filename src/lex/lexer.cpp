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

static char decodeEscapeCharacter(char c) {
	switch(c) {
		case '\'': return '\'';
		case '"': return '"';
		case '\\': return '\\';
		case '?': return '\?';
		case '0': return '\0';
		case 'a': return '\a';
		case 'b': return '\b';
		case 'f': return '\f';
		case 'n': return '\n';
		case 'r': return '\r';
		case 't': return '\t';
		case 'v': return '\v';
	}
	
	return '\0'; //error?
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

    if(!isNonDigit(input->peek())) throw new Exception("Lex Exception: invalid start character in word");

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

    return Token::createIdentifierToken(word, loc);
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
	String str;
	
	if(input->get() != '\"') { // ignore "
		throw new Exception("lexer: expected \"");
	}
	
	while(input->peek() != '\"') {
		if(input->eof()) {
			throw new Exception("expected closing \", found EOF");
		}
	
		if(input->peek() == '\\') {
			input->get();
			str += decodeEscapeCharacter(input->get());
		} else {
			str += input->get();
		}
	}
	
	if(input->get() != '\"') { // ignore "
		throw new Exception("lexer: expected terminating \" in string");
	}
	
	return Token::createStringToken(str, loc);
}


Token Lexer::lexCharLiteral() {
    SourceLocation loc = getLocation();
    assert(input->peek() == '\'');
	
	// ignore opening '
	if(input->get() != '\'') {
		throw new Exception("lexer: expected \' beginning char constant");
	}

	char c;
    if(input->peek() == '\\') {
		input->get();
		c = decodeEscapeCharacter(input->get());
    } else {
		c = input->get();
	}
	
	// ignore closing '
	if(input->get() != '\'') {
		throw new Exception("lexer: expected \' following char constant");
	}
	
    return Token::createCharToken(c, loc);
}
