#ifndef _TOKEN_HPP
#define _TOKEN_HPP

#include "../io/file.hpp"

struct SourceLocation {
    File *file;
    int offset; // character offset into file
                // if column/char info is needed, it can be computed

    SourceLocation();
    SourceLocation(File *_file, int _offset);

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

    bool isLong;

    // if Token data is > 32 bytes, then data is redirected through ptr
    // what the data contains depends on what type of token this is:
    // char literal: value in data[0]
    // string literal: value in data if *short*, else in *dptr
    // numeric literal: value in data
    // identifier: name in data if *short*, else in *dptr
    // keyword: null
    union {
        char data[32];
        void *dptr;
    };

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
    void setStringData(std::string str);
    void setIdentifierName(std::string str);

    char getCharData();
    long getIntData();
    std::string getStringData();
    std::string getIdentifierName();

    tok::TokenKind getKind();
    std::string getKeyword();
    std::string getLiteral();
    std::string getIdentifier();
    SourceLocation getSourceLocation();
};

#endif
