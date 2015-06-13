#include "token.hpp"

#include <assert.h>

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

Token::Token() {
    kind = tok::none;
}

Token::Token(tok::TokenKind _kind, SourceLocation _loc) {
    kind = _kind;
    loc = _loc;
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

void Token::setCharData(char c) {
    data[0] = c;
}

void Token::setIntData(long c) {
	//TODO:
	assert(false);
}

void Token::setStringData(String str) {
	data = str;
}

void Token::setIdentifierName(String str) {
	data = str;
}

char Token::getCharData() {
    assert(kind == tok::charlit);
    return data[0];
}

long Token::getIntData() {
	long ival;
	data.copy((char*) &ival, sizeof(long));
	return ival;
}

String &Token::getStringData() {
	return data;
}

String &Token::getIdentifierName() {
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

