#include "token.hpp"

#include <assert.h>
#include <string.h>
#include <new>
#include "exception/exception.hpp"

SourceLocation::SourceLocation() {
    input = NULL;
    offset = 0;
}

SourceLocation::SourceLocation(Input *_input, int _offset) {
    input = _input;
    offset = _offset;
}

bool SourceLocation::isValid() {
    return input != NULL; //TODO: figure this out later
}

bool Token::hasStringData() const {
	return tag == STRING;
}

Token::Token() {
    kind = tok::none;
	tag = NONE;
}

Token::Token(const Token &t) {
	loc = t.loc;
	kind = t.kind;
	tag = t.tag;
	if(t.hasStringData()) {
		new(strdata) String(*reinterpret_cast<const String *>(t.strdata));
	} else {
		memcpy(strdata, t.strdata, sizeof(strdata)); // just copy bytes
	}
}

Token::Token(tok::TokenKind _kind, SourceLocation _loc) {
    kind = _kind;
    loc = _loc;
	tag = NONE;
}

Token::Token(tok::TokenKind _kind, String _data, SourceLocation _loc) {
	kind = _kind;
	tag = STRING;
	new(strdata) String(_data); // copy string into strdata buffer
	loc = _loc;
}

Token::~Token() {
	if(hasStringData()) {
		String *str = reinterpret_cast<String*>(strdata);
		str->~String(); // destruct string if we are using it
	}
}

Token &Token::operator=(const Token& o) {
	this->~Token();
	loc = o.loc;
	kind = o.kind;
	tag = o.tag;
	if(o.hasStringData()) {
		new(strdata) String(*reinterpret_cast<const String *>(o.strdata));
	} else {
		memcpy(strdata, o.strdata, sizeof(strdata)); // just copy bytes
	}
	return *this;
}

Token Token::createCharToken(char c, SourceLocation _loc) {
	Token tok(tok::charlit, _loc);
	tok.uintdata = c;
	tok.tag = INT;
	return tok;
}

Token Token::createStringToken(String str, SourceLocation _loc) {
	return Token(tok::stringlit, str, _loc);
}

Token Token::createIdentifierToken(String str, SourceLocation _loc) {
	return Token(tok::identifier, str, _loc);
}

Token Token::createIntToken(unsigned long long val, SourceLocation _loc) {
	Token tok(tok::intlit, _loc);
	tok.tag = INT;
	tok.intdata = val;
	return tok;
}

Token Token::createFloatToken(double val, SourceLocation _loc) {
	Token tok(tok::floatlit, _loc);
	tok.floatdata = val;
	tok.tag = FLOAT;
	return tok;
}

Token Token::createCommentToken(String str, SourceLocation _loc) {
	return Token(tok::comment, str, _loc);
}


bool Token::isKeyword() {
	switch(kind) {
#define KEYWORD(NM) case tok::kw_##NM :
#include "tokenkinds.def"
#undef KEYWORD
		return true;
	default:
		return false;
	}
}

bool Token::isLiteral() {
	switch(kind) {
#define LITERAL(NM) case ##NM :
#include "tokenkinds.def"
#undef LITERAL
		return true;
	default:
		return false;
	}
}

bool Token::isIdentifier() {
    return kind == tok::identifier;
}

bool Token::isPunct() {
	switch(kind) {
#define PUNCT(NM, STR) case ##NM :
#include "tokenkinds.def"
#undef PUNCT
			return true;
		default:
			return false;
	}
}

bool Token::is(tok::TokenKind k) {
    return kind == k;
}

bool Token::isNot(tok::TokenKind k) {
    return kind != k;
}

long long Token::getIntData() {
	assert(tag == INT);
	return intdata;
}

unsigned long long Token::getUIntData() {
	assert(tag == INT);
	return uintdata;
}

double Token::getFloatData() {
	assert(tag == FLOAT);
	return floatdata;
}

String &Token::getStringData() {
	if(tag != STRING) throw new Exception("Token does not contain string data");
	return *reinterpret_cast<String*>(strdata);
}

String &Token::getIdentifierName() {
	if(isNot(tok::identifier)) throw new Exception("attempt to get name of non-identifier token");
	return getStringData();
}

tok::TokenKind Token::getKind() {
    return kind;
}

String &Token::getKeyword() {
	throw new Exception("unimplemented");
}

SourceLocation Token::getSourceLocation() {
	return loc;
}

