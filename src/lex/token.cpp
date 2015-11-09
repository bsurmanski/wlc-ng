#include "token.hpp"

#include <assert.h>
#include <string.h>
#include <new>
#include "common/char.hpp"
#include "exception/exception.hpp"

void Token::initializeStringData(String &data) {
    if(hasStringData()) {
        deleteStringData();
    }
    tag = STRING;
	new(strdata) String(data); // copy string into strdata buffer
}

void Token::deleteStringData() {
    String *str = reinterpret_cast<String*>(strdata);
    str->~String(); // destruct string if we are using it
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
        deleteStringData();
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
		return true;
	default:
		return false;
	}
}

bool Token::isTypeKeyword() {
    switch(kind) {
#define TYPE(X) case tok::kw_##X :
#include "tokenkinds.def"
        return true;
        default:
        return false;
    }
}

bool Token::isLiteral() {
	switch(kind) {
#define LITERAL(NM) case ##NM :
#include "tokenkinds.def"
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
#define PUNCT(NM, SYM) case ##NM :
#include "tokenkinds.def"
			return true;
		default:
			return false;
	}
}

bool Token::mayBeBinaryOperator() {
    switch(kind) {
#define BPUNCT(NM, SYM, PREC) case tok:: NM :
#define UBPUNCT(NM, SYM, PREC) case tok:: NM :
#include "tokenkinds.def"
        return true;
        default:
        return false;
    }
}

bool Token::isTerminator() {
    return kind == tok::newline || kind == tok::semicolon;
}

bool Token::is(tok::TokenKind k) {
    return kind == k;
}

bool Token::isNot(tok::TokenKind k) {
    return kind != k;
}

int Token::binaryOperatorPrecidence() {
    switch(kind) {
#define BPUNCT(NM, SYM, PREC) case tok:: NM :\
        return PREC;
#define UBPUNCT(NM, SYM, PREC) case tok:: NM :\
        return PREC;
#include "tokenkinds.def"
        default:
        return 0;
    }
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

String &Token::getKeyword() {
    switch(getKind()) {
#define KEYWORD(X) case tok::kw_ ## X: \
        if(tag != STRING) {\
            String tmp(#X);\
            initializeStringData(tmp);\
        }\
        return getStringData();
#include "tokenkinds.def"
    }

	throw new Exception("Token is not a valid keyword");
}

String &Token::getPunctSymbol() {
    switch(getKind()) {
#define PUNCT(NM, SYM) case tok:: NM: \
        if(tag != STRING) {\
            String tmp( SYM );\
            initializeStringData(tmp);\
        }\
        return getStringData();
#include "tokenkinds.def"
    }

	throw new Exception("Token is not a valid keyword");
}

String &Token::getStringRepr() {
    if(isKeyword()) {
        return getKeyword();
    }

    if(isIdentifier()) {
        return getIdentifierName();
    }

    if(isPunct()) {
        return getPunctSymbol();
    }

    throw new Exception(String("unimplemented token stringify for ") + getKindName());
}

String Token::getKindName() {
    switch(getKind()) {
#define TOK(NM) case tok:: NM: \
        return #NM;
#include "tokenkinds.def"
    }

	throw new Exception("Token is not a valid keyword");
}

tok::TokenKind Token::getKind() {
    return kind;
}


SourceLocation Token::getSourceLocation() {
	return loc;
}

