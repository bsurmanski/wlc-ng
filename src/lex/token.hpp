#ifndef _TOKEN_HPP
#define _TOKEN_HPP

#include "common/string.hpp"
#include "lex/sourceLocation.hpp"

struct SourceRange {
    SourceLocation begin;
    int length;

    SourceRange(SourceLocation loc, int len);
};

namespace tok {
    enum TokenKind {
#define TOK(X) X,
#include "tokenkinds.def"
#undef TOK
		NUM_TOKENS
    };
};

struct Token {
	private:

	union {
		char strdata[sizeof(String)];
		double floatdata;
		long long intdata;
		unsigned long long uintdata;
	};

	enum DataTag {
		NONE,
		STRING,
		FLOAT,
		INT,
	};

	DataTag tag;

    SourceLocation loc;
    tok::TokenKind kind;

    void initializeStringData(String &data);
    void deleteStringData();
	bool hasStringData() const;

    public:
    Token();
	Token(const Token &t);
    Token(tok::TokenKind _kind, SourceLocation _loc);
	Token(tok::TokenKind _kind, String _data, SourceLocation _loc);
	~Token();

	Token &operator=(const Token& o);

	static Token createCharToken(char c, SourceLocation _loc);
	static Token createStringToken(String str, SourceLocation _loc);
	static Token createIdentifierToken(String str, SourceLocation _loc);
	static Token createIntToken(unsigned long long val, SourceLocation _loc); // constant integers cannot be negative (`-123` is parsed as a negative unary op with uint constant)
	static Token createFloatToken(double val, SourceLocation _loc);
	static Token createCommentToken(String str, SourceLocation _loc);

    bool isKeyword();
    bool isTypeKeyword();
    bool isLiteral();
    bool isIdentifier();
    bool isPunct();
    bool mayBeBinaryOperator();
    bool isTerminator();
    bool is(tok::TokenKind k);
    bool isNot(tok::TokenKind k);

    int binaryOperatorPrecidence();

    long long getIntData();
	unsigned long long getUIntData();
	double getFloatData();
    String &getStringData();
    String &getIdentifierName();
    String &getKeyword();
    String &getPunctSymbol();
	String &getStringRepr();

    String getKindName();
    tok::TokenKind getKind();
    SourceLocation getSourceLocation();
};

#endif
