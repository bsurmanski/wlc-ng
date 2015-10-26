#ifndef _PARSER_HPP
#define _PARSER_HPP

#include "program.hpp"
#include "lex/lexer.hpp"
#include "ast/module.hpp"
#include "ast/ast.hpp"
#include "common/buffer.hpp"

class Parser : public Object {
    Program *program;
    Lexer *lex;

    Module *module;

    Buffer<Token> tbuf;

    public:
    Parser(Program *_program, Lexer *_lex);
    ~Parser();
    Module *parse();
    Lexer *getLexer();

    protected:
    Token &peekTok(int i = 0);
    bool ignoreTok();
    Token getTok();

    public:
    Stmt *parseStmt();
    Expr *parseExpr();
    Decl *parseDecl();
    Type *parseType();

    CompoundStmt        *parseCompoundStmt();
    LabelStmt           *parseLabelStmt();
    GotoStmt            *parseGotoStmt();
    BreakStmt           *parseBreakStmt();
    ContinueStmt        *parseContinueStmt();
    ReturnStmt          *parseReturnStmt();
    CaseStmt            *parseCaseStmt();
    IfStmt              *parseIfStmt();

    BinaryExpr          *parseBinaryExpr();
    UnaryExpr           *parseUnaryExpr();
    CharLiteralExpr     *parseCharLiteralExpr();
    StringLiteralExpr   *parseStringLiteralExpr();
    BoolLiteralExpr     *parseBoolLiteralExpr();
    NullLiteralExpr     *parseNullLiteralExpr();
    NumericLiteralExpr  *parseNumericLiteralExpr();
    TupleExpr           *parseTupleExpr();
    NewExpr             *parseNewExpr();
    DeleteExpr          *parseDeleteExpr();
    RetainExpr          *parseRetainExpr();
    ReleaseExpr         *parseReleaseExpr();
    IdExpr              *parseIdExpr();
    PackExpr            *parsePackExpr();
    MemberExpr          *parseMemberExpr();
    CallExpr            *parseCallExpr();
    IndexExpr           *parseIndexExpr();
    CastExpr            *parseCastExpr();

    PrimativeType       *parsePrimativeType();
    UserType            *parseUserType();
    TupleType           *parseTupleType();
    FunctionType        *parseFunctionType();
    PointerType         *parsePointerType();
    ArrayType           *parseArrayType();
};

#endif
