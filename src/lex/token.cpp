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
	tok.tag = UINT;
	return tok;
}

Token Token::createStringToken(String str, SourceLocation _loc) {
	return Token(tok::stringlit, str, _loc);
}

Token Token::createIdentifierToken(String str, SourceLocation _loc) {
	return Token(tok::identifier, str, _loc);
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
	if(tag != STRING) throw new Exception("Token does not contain string data");
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

