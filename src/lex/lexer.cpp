#include "lexer.hpp"
#include "common/char.hpp"
#include "exception/exception.hpp"

#include <assert.h>
#include <math.h>
#include <stdio.h>

using namespace Char;

enum NumericFormat {
	NUM_BIN,
	NUM_OCT,
	NUM_DEC,
	NUM_HEX,
};

static double makeFloat(long long integral, double fraction, double exponent) {
	return (integral + fraction) * exponent;
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
	} while (tok.is(tok::whitespace) || tok.is(tok::comment));
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

    if (c == '"' || c == '`') {
        return lexStringLiteral();
    }

    if(c == '\'') {
        return lexCharLiteral();
    }

	if(c == '/') {
		char c1 = input->peek(1);
		if(c1 == '*') {
			return lexBlockComment();
		}

		if(c1 == '/') {
			return lexLineComment();
		}
	}

	if(isPunctuatorChar(c)) {
		return lexPunctuator();
	}

    if(isWhitespace(c)) {
        SourceLocation loc = getLocation();
        do {
            input->get();
        } while(isWhitespace(input->peek()));
        return Token(tok::whitespace, loc);
    }

    if(isEndOfLine(c)) {
        SourceLocation loc = getLocation();
        input->get();
        return Token(tok::newline, loc);
    }

	throw new Exception("invalid input character found");
}


bool Lexer::eof() {
	return input->eof();
}


SourceLocation Lexer::getLocation() {
    return SourceLocation(input, input->tell());
}

String Lexer::lexUTF8Char() {
    String str;
    long long val = consumeHexSeq();

    char c;
    if(val < 0x80) {
        str.append((char) val);
    } else if(val < 0x800) {
        str.append((char) (0xC0 | ((val >> 6) & 0x3F)));
        str.append((char) (0x80 | (val & 0x3F)));
    } else if(val < 0x10000) {
        str.append((char) (0xE0 | ((val >> 12) & 0x3F)));
        str.append((char) (0x80 | ((val >> 6) & 0x3F)));
        str.append((char) (0x80 | ((val & 0x3F))));
    } else if(val <= 0x10FFFF) {
        str.append((char) (0xF0 | ((val >> 18) & 0x3F)));
        str.append((char) (0x80 | ((val >> 12) & 0x3F)));
        str.append((char) (0x80 | ((val >> 6) & 0x3F)));
        str.append((char) (0x80 | ((val & 0x3F))));
    } else {
        throw new Exception("UTF8 constant must be <= 0x10FFFF");
    }

    return str;
}

String Lexer::lexEscapeSequence() {
    assert(input->get() == '\\' && "expected '\\' beginning escape sequence");

    char c = input->get();

    // hex escape sequence
    if(c == 'x') {
        String str;
        char c = 0;
        int count = 0;
        while(isHexDigit(input->peek())) {
            c <<= 4;
            c += hextoi(input->get());

            if(count == 1) {
                str.append((char) c);
                c = 0;
            }

            count = (count + 1) % 2;
        }

        // there is left over bits at the end that we have buffered
        // left justify them in the byte and append to string
        if(count) {
            c <<= 4;
            str.append((char) c);
        }

        return str;
    }

    // oct escape sequence
    if(c == 'o') {
        String str;
        int c = 0;
        int count = 0;
        while(isOctDigit(input->peek())) {
            c <<= 3;
            c += octtoi(input->get());

            count += 3;

            if(count > 8) {
                str.append((char) (c & 0xff));
                c >>= 8;
                count -= 8;
            }
        }

        // there is left over bits at the end that we have buffered
        // left justify them in the byte and append to string
        if(count) {
            c <<= (8 - count);
            str.append((char) c);
        }

        return str;
    }

    // decimal escape sequence
    if(c == 'd') {
        //TODO
        throw new Exception("lexer: decimal escape sequence not supported");
    }

    // binary escape sequence
    if(c == 'b') {
        String str;
        char c = 0;
        int count = 0;
        while(isBinDigit(input->peek())) {
            c <<= 1;
            c += bintoi(input->get());

            if(count == 7) {
                str.append((char) c);
                c = 0;
            }

            count = (count + 1) % 8;
        }

        // there is left over bits at the end that we have buffered
        // left justify them in the byte and append to string
        if(count) {
            c <<= (8 - count);
            str.append((char) c);
        }
        return str;
    }

    // unicode escape sequence
    if(c == 'u') {
        return lexUTF8Char();
    }

	switch(c) {
		case '\'': return String("'");
		case '"': return String("\"");
		case '\\': return String("\\");
		case '?': return String("\?");
		case '0': return String("\0");
		case 'f': return String("\f");
		case 'n': return String("\n");
		case 'r': return String("\r");
		case 't': return String("\t");
        case ' ': return String();
	}

	return String();
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

unsigned long long Lexer::consumeDecSeq(int *len_out) {
	int len = 0;
    unsigned long long ret = 0;
    while(isDigit(input->peek()) || input->peek() == '_') {
		if(input->peek() == '_') {
			input->get();
		} else {
			ret *= 10;
			ret += dectoi(input->get());
			len++;
		}
    }

	if(len_out) *len_out = len;

	return ret;
}

unsigned long long Lexer::consumeHexSeq(int *len_out) {
	int len = 0;
	unsigned long long ret = 0;

    while(isHexDigit(input->peek()) || input->peek() == '_') {
		if(input->peek() == '_') {
			input->get();
		} else {
			char c = input->get();
			ret <<= 4; // each hex digit is 4 bits
			ret += hextoi(c);
			len++;
		}
    }

	if(len_out) *len_out = len;

	return ret;
}

unsigned long long Lexer::consumeOctSeq(int *len_out) {
	int len = 0;
	unsigned long long ret = 0;
    while(isOctDigit(input->peek()) || input->peek() == '_') {
		if(input->peek() == '_') {
			input->get();
		} else {
			ret <<= 3; // each oct digit is 3 bits
			ret += octtoi(input->get());
			len++;
		}
    }

	if(len_out) *len_out = len;

	return ret;
}

unsigned long long Lexer::consumeBinSeq(int *len_out) {
	int len = 0;
	unsigned long long ret = 0;
    while(isBinDigit(input->peek()) || input->peek() == '_') {
		if(input->peek() == '_') {
			input->get();
		} else {
			ret <<= 1; // each bin digit is 1 bit
			ret += bintoi(input->get());
			len++;
		}
    }

	if(len_out) *len_out = len;

	return ret;
}

Token Lexer::lexNumericLiteral() {
	SourceLocation loc = getLocation();

	NumericFormat format = NUM_DEC;
	bool floating = false;
	long long integral = 0;
	int fraclen = 0;
	double fraction = 0.0;
	long long fracdigs = 0;
	double exponent = 1;

    String num;

    if(input->peek() == '0') {
		input->get(); // ignore first leading zero
        char c = input->peek();
        if(c == 'x') {
			format = NUM_HEX;
            input->get();
            integral = consumeHexSeq();
        } else if(c == 'o') {
			format = NUM_OCT;
            input->get();
            integral = consumeOctSeq();
        } else if(c == 'b') {
			format = NUM_BIN;
            input->get();
            integral = consumeBinSeq();
        } else {
			integral = consumeDecSeq();
		}
    } else {
		integral = consumeDecSeq();
	}

	if(input->peek() == '.') {
		input->get(); // ignore .
		floating = true;
		if(format == NUM_HEX) {
			fracdigs = consumeHexSeq(&fraclen);
			if(fraclen) fraction = fracdigs / pow(16, fraclen);
		} else if(format == NUM_OCT) {
			fracdigs = consumeOctSeq(&fraclen);
			if(fraclen) fraction = fracdigs / pow(8, fraclen);
		} else if(format == NUM_BIN) {
			fracdigs = consumeBinSeq(&fraclen);
			if(fraclen) fraction = fracdigs / pow(2, fraclen);
		} else {
			fracdigs = consumeDecSeq(&fraclen);
			if(fraclen) fraction = fracdigs / pow(10, fraclen);
		}
	}

	// exponent part
	if(isExponentChar(input->peek())) {
		char expChar = input->get();
		bool negativeExp = false;

		floating = true;
		if(isSign(input->peek())) {
			if(input->get() == '-') {
				negativeExp = true;
			}
		}

		long long expSeq = consumeDecSeq();
		if(negativeExp) expSeq *= -1;
		exponent = (isBinExponentChar(expChar) ? pow(2, expSeq) : pow(10, expSeq));
	}

	if(!isWhitespace(input->peek()) && !isEndOfLine(input->peek()) && !input->eof()) {
		throw new Exception("invalid trailing characters on numeric constant: " + input->get());
	}

	if(floating) {
		return Token::createFloatToken(makeFloat(integral, fraction, exponent), loc);
	} else {
		return Token::createIntToken(integral, loc);
	}
}

//TODO: deal with whitespace
Token Lexer::lexStringLiteral() {
	SourceLocation loc = getLocation();
    char c = input->peek();
    String str;
    while(c == '"' || c == '`') {
        if(c == '"') {
            str.append(lexNormalStringLiteralPart());
        } else if(c == '`') {
            str.append(lexRawStringLiteralPart());
        }
        c = input->peek();
    }
	return Token::createStringToken(str, loc);
}

String Lexer::lexNormalStringLiteralPart() {
    char c;
	String str;

	if((c = input->get()) != '\"') { // ignore "
        String error = "lexer: expected \", found ";
        error.append(c);
		throw new Exception(error);
	}

	while((c = input->peek()) != '\"') {
		if(input->eof()) {
			throw new Exception("expected closing \", found EOF");
		}

		if(input->peek() == '\\') {
            String escape = lexEscapeSequence();
            str.append(escape);
		} else {
			str += input->get();
		}
	}

	if((c = input->get()) != '\"') { // ignore "
		throw new Exception("lexer: expected terminating \" in string");
	}

	return str;
}


String Lexer::lexRawStringLiteralPart() {
	String str;

	if(input->get() != '`') { // ignore `
		throw new Exception("lexer: expected `");
	}

	while(input->peek() != '`') {
		if(input->eof()) {
			throw new Exception("expected closing `, found EOF");
		}

		str += input->get();
	}

	if(input->get() != '`') { // ignore `
		throw new Exception("lexer: expected terminating ` in raw string");
	}

	return str;
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
        String escape = lexEscapeSequence();
        if(escape.length() < 1) throw new Exception("lexer: invalid empty character literal");
        if(escape.length() > 1) throw new Exception("lexer: escape sequence is greater than one byte long");
        c = escape.charAt(0);
    } else {
		c = input->get();
	}

	// ignore closing '
	if(input->get() != '\'') {
		throw new Exception("lexer: expected \' following char constant");
	}

    if(!c) {
		throw new Exception("lexer: invalid empty character literal");
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


Token Lexer::lexBlockComment() {
	SourceLocation loc = getLocation();
	String str;

	input->get();
	input->get(); // ignore /*

	int level = 1;
	char c;
	do {
		c = input->get();
		if(c == '/' && input->peek() == '*') {
			level++;
			str += c;
			str += input->get();
			continue;
		}

		if(c == '*' && input->peek() == '/') {
			level--;
			if(level <= 0) break;

			str += c;
			str += input->get();
			continue;
		}

		str += c;

		if(input->eof() && level > 0) throw new Exception("expected closing '*/' for block comment");

	} while(true);

	return Token::createCommentToken(str, loc);
}

Token Lexer::lexLineComment() {
	SourceLocation loc = getLocation();
	String str;

	input->get();
	input->get(); // ignore //

	do {
		str += input->get();
	} while(input->peek() != '\n' && !input->eof());

	return Token::createCommentToken(str, loc);
}
