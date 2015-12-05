#ifndef _PARSER_HPP
#define _PARSER_HPP

#include "ast/ast.hpp"
#include "common/buffer.hpp"
#include "common/object.hpp"
#include "lex/token.hpp"

class Lexer;
class Program;
class Module;

class Parser : public Object {
    int error;

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
    int ignoreNewlines();
    int ignoreStmtTerminators();
    bool ignoreTok();
    Token getTok();

    void emit_error(SourceLocation loc, String err);

    public:
    Stmt *parseStmt();
    Expr *parseExpr();
    Decl *parseDecl();
    Type *parseType();

    Stmt                *parseAssignPostfix(Expr *lhs);
    CompoundStmt        *parseCompoundStmt();
    LabelStmt           *parseLabelStmt();
    GotoStmt            *parseGotoStmt();
    BreakStmt           *parseBreakStmt();
    ContinueStmt        *parseContinueStmt();
    ReturnStmt          *parseReturnStmt();
    CaseStmt            *parseCaseStmt();
    IfStmt              *parseIfStmt();
    WhileStmt           *parseWhileStmt();
    DoWhileStmt         *parseDoWhileStmt();
    ForStmt             *parseForStmt();
    ForEachStmt         *parseForEachStmt();

    Expr                *parsePostfixExpr(Expr *expr);
    Expr                *parsePrimaryExpr();
    Expr                *parseBinaryExpr(int precidence = 0);
    Expr                *parseUnaryExpr();
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
    DynArray<Expr*>     parseExprList();
    CallExpr            *parseCallExprPostfix(Expr *expr);
    IndexExpr           *parseIndexExpr();
    CastExpr            *parseCastExpr();

    TypeDecl            *parseTypeDecl();
    DynArray<VarDecl*>   parseParams();
    Decl                *parseNonTypeDecl();

    PrimativeType       *parsePrimativeType();
    IdType              *parseIdType();
    TupleType           *parseTupleType();
    FunctionType        *parseFunctionType();
    PointerType         *parsePointerType();
    ArrayType           *parseArrayType();

    Type                *parseTypePostfix(Type *base);
};

#endif
