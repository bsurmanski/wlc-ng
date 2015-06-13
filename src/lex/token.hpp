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
        NUM_TOKENS
#undef TOK
    };
};

struct Token {
    private:
    SourceLocation loc;
    tok::TokenKind kind;

    String data;

    public:
    Token();
    Token(tok::TokenKind _kind, SourceLocation _loc);

    bool isKeyword();
    bool isLiteral();
    bool isIdentifier();
    bool isPunct();
    bool is(tok::TokenKind k);
    bool isNot(tok::TokenKind k);

    void setCharData(char c);
    void setIntData(long c);
    void setStringData(String str);
    void setIdentifierName(String str);

    char getCharData();
    long getIntData();
    String &getStringData();
    String &getIdentifierName();

    tok::TokenKind getKind();
    String getKeyword();
    String getLiteral();
    String &getIdentifier();
    SourceLocation getSourceLocation();
};

#endif
