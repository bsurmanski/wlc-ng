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

static bool isWhitespace(char c) {
	return c == ' ' || c == '\t' || c == '\v' || c == '\f';
}

static bool isEndOfLine(char c) {
	return c == '\n' || c == '\r';
}

static bool isPunctuatorChar(char c) {
	switch(c) {
		case '~':
		case '!':
		case '%':
		case '^':
		case '&':
		case '*':
		case '(':
		case ')':
		case '-':
		case '+':
		case '=':
		case '[':
		case ']':
		case '{':
		case '}':
		case '|':
		case '/':
		case '<':
		case '>':
		case ';':
		case ':':
		case '.':
		case ',':
			return true;
		default:
			return false;
	}
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
	Token tok;
	do {
		if(eof()) {
			return Token(tok::eof, getLocation());
		}
		
		tok = lexRaw();
	} while (tok.is(tok::whitespace));
	return tok;
}

Token Lexer::lexRaw() {
    char c = input->peek();
    if(isDigit(c)) {
        return lexNumericLiteral();
    }

    if(isNonDigit(c)) {
        return lexWord();
    }
	
	if(isPunctuatorChar(c)) {
		return lexPunctuator();
	}

    if(c == '"') {
        return lexStringLiteral();
    }

    if(c == '\'') {
        return lexCharLiteral();
    }

	throw new Exception("invalid input character found");
}


bool Lexer::eof() {
	return input->eof();
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


Token Lexer::lexPunctuator() {
	SourceLocation loc = getLocation();
	String str;
	str += input->get();
	
	#define OPT_CONSUME(C, DO) {if(input->peek() == C) { str += input->get(); DO; }}
	
	switch(str[0]) {
		case '~':
			return Token(tok::tilde, str, loc);
		case '!':
			OPT_CONSUME('=', return Token(tok::bangequal, str, loc));
			return Token(tok::bang, str, loc);
		case '%':
			OPT_CONSUME('=', return Token(tok::percentequal, str, loc));
			return Token(tok::percent, str, loc);
		case '^':
			OPT_CONSUME('=', return Token(tok::caretequal, str, loc));
			return Token(tok::caret, str, loc);
		case '&':
			OPT_CONSUME('=', return Token(tok::ampequal, str, loc));
			OPT_CONSUME('&', return Token(tok::ampamp, str, loc));
			return Token(tok::amp, str, loc);
		case '*':
			OPT_CONSUME('=', return Token(tok::starequal, str, loc));
			OPT_CONSUME('*', return Token(tok::starstar, str, loc));
			OPT_CONSUME('/', return Token(tok::starslash, str, loc));
			return Token(tok::star, str, loc);
		case '(':
			return Token(tok::lparen, str, loc);
		case ')':
			return Token(tok::rparen, str, loc);
		case '-':
			OPT_CONSUME('=', return Token(tok::minusequal, str, loc));
			OPT_CONSUME('-', return Token(tok::minusminus, str, loc));
			return Token(tok::minus, str, loc);
		case '+':
			OPT_CONSUME('=', return Token(tok::plusequal, str, loc));
			OPT_CONSUME('+', return Token(tok::plusplus, str, loc));
			return Token(tok::plus, str, loc);
		case '=':
			OPT_CONSUME('=', return Token(tok::equalequal, str, loc));
			return Token(tok::equal, str, loc);
		case '[':
			return Token(tok::lbracket, str, loc);
		case ']':
			return Token(tok::rbracket, str, loc);
		case '{':
			return Token(tok::lbrace, str, loc);
		case '}':
			return Token(tok::rbrace, str, loc);
		case '|':
			OPT_CONSUME('=', return Token(tok::barequal, str, loc));
			OPT_CONSUME('|', return Token(tok::barbar, str, loc));
			return Token(tok::bar, str, loc);
		case '/':
			OPT_CONSUME('/', return Token(tok::slashslash, str, loc));
			OPT_CONSUME('*', return Token(tok::slashstar, str, loc));
			OPT_CONSUME('=', return Token(tok::slashequal, str, loc));
			return Token(tok::slash, str, loc);
			
		case '<':
			OPT_CONSUME('=', return Token(tok::lessequal, str, loc));
			OPT_CONSUME('<', return Token(tok::lessless, str, loc));
			return Token(tok::less, str, loc);
			
		case '>':
			OPT_CONSUME('=', return Token(tok::greaterequal, str, loc));
			OPT_CONSUME('>', return Token(tok::greatergreater, str, loc));
			return Token(tok::greater, str, loc);
		case ':':
			OPT_CONSUME('=', return Token(tok::colonequal, str, loc));
			return Token(tok::colon, str, loc);
			
		case ';':
			return Token(tok::semicolon, str, loc);
			
		case '.':
			OPT_CONSUME('.', OPT_CONSUME('.', return Token(tok::dotdotdot, str, loc)); return Token(tok::dotdot, str, loc));
			return Token(tok::dot, str, loc);
			
		case ',':
			return Token(tok::comma, str, loc);
				
		default:
			String err = String("lexer: unknown punctuator character in input: ");
			err += str[0];
			throw new Exception(err);
	}
}