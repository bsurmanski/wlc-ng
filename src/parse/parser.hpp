#ifndef _PARSER_HPP
#define _PARSER_HPP

#include "../program.hpp"
#include "../lex/lexer.hpp"
#include "../ast/module.hpp"
#include "../ast/ast.hpp"

class Parser {
    Program *program;
    Lexer *lex;

    Module *module;

    Token peek;

    public:
    Parser(Program *_program, Lexer *_lex);
    ~Parser();
    Module *parse();

    protected:
    Token &peekTok();
    void ignoreTok();
    Token getTok();

    Stmt *parseStmt();
    Expr *parseExpr();
    Decl *parseDecl();

    CompoundStmt    *parseCompoundStmt();
    ReturnStmt      *parseReturnStmt();
};

#endif
