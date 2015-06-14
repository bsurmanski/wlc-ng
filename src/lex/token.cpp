#include "token.hpp"

#include <assert.h>
#include <string.h>
#include <new>

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
	return kind == tok::identifier || kind == tok::stringlit;
}

Token::Token() {
    kind = tok::none;
}

Token::Token(const Token &t) {
	loc = t.loc;
	kind = t.kind;
	if(t.hasStringData()) {
		new(strdata) String(*reinterpret_cast<const String *>(t.strdata));
	} else {
		memcpy(strdata, t.strdata, sizeof(strdata)); // just copy bytes
	}
}

Token::Token(tok::TokenKind _kind, SourceLocation _loc) {
    kind = _kind;
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
	return tok;
}

Token Token::createStringToken(String str, SourceLocation _loc) {
	Token tok(tok::stringlit, _loc);
	new(tok.strdata) String(str); // copy string into strdata buffer
	return tok;
}

Token Token::createIdentifierToken(String str, SourceLocation _loc) {
	Token tok(tok::identifier, _loc);
	new(tok.strdata) String(str); // copy string into strdata buffer
	return tok;
}

Token Token::createIntToken(long long val, SourceLocation _loc) {
}

Token Token::createUIntToken(unsigned long long val, SourceLocation _loc) {
}

Token Token::createFloatToken(double val, SourceLocation _loc) {
}


bool Token::isKeyword() {
}

bool Token::isLiteral() {
}

bool Token::isIdentifier() {
    return kind == tok::identifier;
}

bool Token::isPunct() {
}

bool Token::is(tok::TokenKind k) {
    return kind == k;
}

bool Token::isNot(tok::TokenKind k) {
    return kind != k;
}

long long Token::getIntData() {
	return intdata;
}

unsigned long long Token::getUIntData() {
	return uintdata;
}

String &Token::getStringData() {
	return *reinterpret_cast<String*>(strdata);
}

String &Token::getIdentifierName() {
	return *reinterpret_cast<String*>(strdata);
}

tok::TokenKind Token::getKind() {
    return kind;
}

String Token::getKeyword() {
}

String Token::getLiteral() {
}

String &Token::getIdentifier() {
}

SourceLocation Token::getSourceLocation() {
	return loc;
}

