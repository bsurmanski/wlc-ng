#include "token.hpp"

#include <assert.h>

SourceLocation::SourceLocation() {
    file = NULL;
    offset = 0;
}

SourceLocation::SourceLocation(File *_file, int _offset) {
    file = _file;
    offset = _offset;
}

bool SourceLocation::isValid() {
    return file != NULL; //TODO: figure this out later
}

Token::Token() {
    kind = tok::none;
    isLong = false;
}

Token::Token(tok::TokenKind _kind, SourceLocation _loc) {
    kind = _kind;
    loc = _loc;
    isLong = false;
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
}

void Token::setStringData(std::string str) {
}

void Token::setIdentifierName(std::string str) {
}

char Token::getCharData() {
    assert(kind == tok::charlit);
    return data[0];
}

long Token::getIntData() {
}

std::string Token::getStringData() {
}

std::string Token::getIdentifierName() {
}

tok::TokenKind Token::getKind() {
    return kind;
}

std::string Token::getKeyword() {
}

std::string Token::getLiteral() {
}

std::string Token::getIdentifier() {
}

SourceLocation Token::getSourceLocation() {
}

