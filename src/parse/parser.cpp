#include "parser.hpp"

#include <assert.h>

Parser::Parser(Program *_program, Lexer *_lex) : program(_program), lex(_lex), peek(lex->lex()) {
}

Parser::~Parser() {
    delete program;
    delete lex;
}

Module *Parser::parse() {
    Module *module = new Module;

    return module;
}

Token &Parser::peekTok() {
    return peek;
}

void Parser::ignoreTok() {
    peek = lex->lex();
}

Token Parser::getTok() {
    Token t = peek;
    peek = lex->lex();
    return t;
}

Stmt *Parser::parseStmt() {
    Token token = peekTok();

    switch(token.getKind()) {
        case tok::lbrace:
            return parseCompoundStmt();
        case tok::kw_goto:
        case tok::kw_break:
            ignoreTok();
            return new BreakStmt;

        case tok::kw_continue:
            ignoreTok();
            return new ContinueStmt;

        case tok::kw_return:
            return parseReturnStmt();
    }
}

CompoundStmt *Parser::parseCompoundStmt() {
}

ReturnStmt *Parser::parseReturnStmt() {
    assert(peekTok().getKind() == tok::kw_return);
    ignoreTok();
    Expr *ex = parseExpr();
    //TODO
}

Expr *Parser::parseExpr() {
}

Decl *Parser::parseDecl() {
}
