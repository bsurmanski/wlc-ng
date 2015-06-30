#ifndef _TOKEN_HPP
#define _TOKEN_HPP

#include "io/file.hpp"
#include "common/string.hpp"

struct SourceLocation {
    Input *input;
    int offset; // character offset into file
                // if column/char info is needed, it can be computed

    SourceLocation();
    SourceLocation(Input *_input, int _offset);

    bool isValid();
};

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

    bool isKeyword();
    bool isLiteral();
    bool isIdentifier();
    bool isPunct();
    bool is(tok::TokenKind k);
    bool isNot(tok::TokenKind k);

    long long getIntData();
	unsigned long long getUIntData();
	double getFloatData();
    String &getStringData();
    String &getIdentifierName();
	String &getStringRepr();

    tok::TokenKind getKind();
    String &getKeyword();
    SourceLocation getSourceLocation();
};

#endif
