#include "parser.hpp"

#include "program.hpp"
#include "exception/exception.hpp"
#include "exception/parseException.hpp"
#include "ast/module.hpp"
#include "lex/lexer.hpp"

#include <assert.h>

Parser::Parser(Program *_program, Lexer *_lex) : program(_program), lex(_lex) {
}

Parser::~Parser() {
    while(!tbuf.empty()) {
        tbuf.pop();
    }
    program->release();
    lex->release();
}

Module *Parser::parse() {
    Module *module = new Module;

    return module;
}

Lexer *Parser::getLexer() {
    return lex;
}

Token &Parser::peekTok(int i) {
    while(tbuf.length() < i+1) {
        tbuf.push(lex->lex());
    }

    return tbuf.peek(i);
}

bool Parser::ignoreTok() {
    if(tbuf.empty()) {
        lex->lex();
    } else {
        tbuf.pop();
    }
    return true;
}

Token Parser::getTok() {
    if(!tbuf.empty()) {
        return tbuf.pop();
    } else {
        return lex->lex();
    }
}

/*
 * Stmt
 */

Stmt *Parser::parseStmt() {
    Token &token = peekTok();

    Stmt *stmt = NULL;

    switch(token.getKind()) {
        case tok::lbrace:
            stmt = parseCompoundStmt();
            break;

        case tok::kw_label:
           stmt = parseLabelStmt();
           break;

        case tok::kw_goto:
            stmt = parseGotoStmt();
            break;

        case tok::kw_break:
            stmt = parseBreakStmt();
            break;

        case tok::kw_continue:
            stmt = parseContinueStmt();
            break;

        case tok::kw_return:
            stmt = parseReturnStmt();
            break;

        case tok::kw_case:
            stmt = parseCaseStmt();
            break;

        case tok::kw_if:
            stmt = parseIfStmt();
            break;

        //TODO: use_stmt, import_stmt

        default:
            stmt = parseExpr();

            //TODO: augmented assign, eg: +=, -=, *=
            if(peekTok().is(tok::equal)) {
                ignoreTok(); // ignore '='
                stmt = new AssignStmt((Expr*) stmt, parseExpr());
            }
            break;
    }

    do {
        if(!peekTok().isTerminator() && !peekTok().is(tok::eof)) {
            throw new ParseException(peekTok().getSourceLocation(), String("Expected line-terminator"));
        }
    } while(peekTok().isTerminator());

    //TODO: stmt terminator
    return stmt;
}

CompoundStmt *Parser::parseCompoundStmt() {
    if(peekTok().getKind() != tok::lbrace || !ignoreTok()) {
        throw new ParseException(peekTok().getSourceLocation(), "expected left brace beginning compound stmt");
    }

    DynArray<Stmt*> stmts;
    while(peekTok().getKind() != tok::rbrace) {
        Stmt *stmt = parseStmt();
        stmts.append(stmt);
    }

    if(peekTok().getKind() != tok::lbrace || !ignoreTok()) {
        throw new ParseException(peekTok().getSourceLocation(), "expected right brace following compound stmt");
    }

    return new CompoundStmt(stmts);
}

LabelStmt *Parser::parseLabelStmt() {
    if(peekTok().getKind() != tok::kw_label || !ignoreTok()) {
        throw new ParseException(peekTok().getSourceLocation(), "expected 'label' beginning label stmt");
    }

    Token tok = getTok();
    if(tok.getKind() != tok::identifier) {
        throw new ParseException(tok.getSourceLocation(), "expected identifier following 'label' keyword");
    }

    //TODO: line terminator

    return new LabelStmt(tok.getIdentifierName());
}

GotoStmt *Parser::parseGotoStmt() {
    if(peekTok().getKind() != tok::kw_goto || !ignoreTok()) {
        throw new ParseException(peekTok().getSourceLocation(), "expected 'goto' beginning goto stmt");
    }

    Token tok = getTok();
    if(tok.getKind() != tok::identifier) {
        throw new ParseException(tok.getSourceLocation(), "expected identifier following 'goto' keyword");
    }

    //TODO: line terminator

    return new GotoStmt(tok.getIdentifierName());
}

BreakStmt *Parser::parseBreakStmt() {
    if(peekTok().getKind() != tok::kw_break || !ignoreTok()) {
        throw new ParseException(peekTok().getSourceLocation(), "expected 'break' beginning break stmt");
    }

    //TODO: line terminator

    return new BreakStmt;
}

ContinueStmt *Parser::parseContinueStmt() {
    if(peekTok().getKind() != tok::kw_continue || !ignoreTok()) {
        throw new ParseException(peekTok().getSourceLocation(), "expected 'continue' beginning continue stmt");
    }

    //TODO: line terminator

    return new ContinueStmt;
}

ReturnStmt *Parser::parseReturnStmt() {
    if(peekTok().getKind() != tok::kw_return || !ignoreTok()) {
        throw new ParseException(peekTok().getSourceLocation(), "expected 'return' keyword");
    }

    //TODO: check line terminator before trying to read value
    Expr *expr = parseExpr();
    //TODO: line termnator
    return new ReturnStmt(expr);
}

CaseStmt *Parser::parseCaseStmt() {
    throw new ParseException(peekTok().getSourceLocation(), "unimplemented: parse case stmt");
}

IfStmt *Parser::parseIfStmt() {
    if(peekTok().getKind() != tok::kw_if || !ignoreTok()) {
        throw new ParseException(peekTok().getSourceLocation(), "expected 'if' keyword beginning if stmt");
    }

    if(peekTok().getKind() != tok::lparen || !ignoreTok()) {
        throw new ParseException(peekTok().getSourceLocation(), "expected '(' in if stmt");
    }

    Expr *cond_expr = parseExpr();

    if(peekTok().getKind() != tok::rparen || !ignoreTok()) {
        throw new ParseException(peekTok().getSourceLocation(), "expected ')' in if stmt");
    }

    Stmt *cond_stmt = parseStmt();
    Stmt *else_stmt = NULL;

    if(peekTok().getKind() == tok::kw_else) {
        ignoreTok();
        else_stmt = parseStmt();
    }

    //TODO: return new IfStmt(cond_expr, cond_stmt, else_stmt);

    throw new ParseException(peekTok().getSourceLocation(), "unimplemented: parse if stmt");
}

/*
 * Expr
 */
Expr *Parser::parseExpr() {
    Expr *expr = NULL;

    switch(peekTok().getKind()) {
#define UBPUNCT(X, Y, Z) case ##X:
#define UPUNCT(X, Y) case ##X:
#include "lex/tokenkinds.def"
        return parseUnaryExpr();

        case tok::lbracket:
        case tok::lparen:
        case tok::floatlit:
        case tok::intlit:
        case tok::stringlit:
        case tok::charlit:
        case tok::kw_null:
        case tok::kw_true:
        case tok::kw_false:
        case tok::identifier:
        return parsePrimaryExpr();

        default:
        throw new ParseException(peekTok().getSourceLocation(), "invalid expression");
    }

    throw new ParseException(peekTok().getSourceLocation(), "unimplemented: parse expr");
}

Expr *Parser::parsePrimaryExpr() {
    Token tok = getTok();
    Expr *expr;
    switch(tok.getKind()) {
        case tok::lbracket:
            throw new ParseException(peekTok().getSourceLocation(), "unimplemented: parse tuple expr");

        case tok::lparen:
            expr = parseExpr();
            if(peekTok().getKind() != tok::rparen && ignoreTok()) {
                throw new ParseException(peekTok().getSourceLocation(), "expected )");
            }
            return expr;

        case tok::kw_null:
            return new NullLiteralExpr;

        case tok::kw_true:
            return new BoolLiteralExpr(true);

        case tok::kw_false:
            return new BoolLiteralExpr(false);

        case tok::identifier:
            return new IdExpr(tok.getIdentifierName());

        case tok::floatlit:
        case tok::intlit:
            return new IntLiteralExpr(tok.getIntData());
        case tok::stringlit:
            return new StringLiteralExpr(tok.getStringData());
        case tok::charlit:

        default:
            throw new ParseException(peekTok().getSourceLocation(), String("invalid or unimplemented primary expr: ") + tok.getStringRepr());
    }
}

BinaryExpr *Parser::parseBinaryExpr(int precidence) {
    Expr *lhs = parsePrimaryExpr();
    throw new ParseException(peekTok().getSourceLocation(), "unimplemented: parse binexpr");
}

UnaryExpr *Parser::parseUnaryExpr() {
    Token op = getTok();
    switch(op.getKind()) {
        case tok::plus:
            throw new ParseException(peekTok().getSourceLocation(), "unimplemented: parse unary +");
        case tok::plusplus:
            return new PreIncExpr(parseExpr());
        case tok::minus:
            return new NegateExpr(parseExpr());
        case tok::minusminus:
            return new PreDecExpr(parseExpr());
        case tok::amp:
            return new RefExpr(parseExpr());
        case tok::caret:
            return new DerefExpr(parseExpr());
        case tok::bang:
            return new NotExpr(parseExpr());
        case tok::tilde:
            throw new ParseException(peekTok().getSourceLocation(), "unimplemented: parse unary ~");
            break;
        default:
        throw new ParseException(peekTok().getSourceLocation(), String("invalid unary operator: ") + op.getStringRepr());
    }
}

/*
 * Decl
 */
Decl *Parser::parseDecl() {
    throw new ParseException(peekTok().getSourceLocation(), "unimplemented: parse decl");
}

/*
 * Type
 */

Type *Parser::parseType() {
    switch(peekTok().getKind()) {
        case tok::lbracket:
            // tuple
        case tok::identifier:
            break;
    }
}

PrimativeType *Parser::parsePrimativeType() {
    Token &token = peekTok();
    PrimativeType *type = NULL;

    switch(token.getKind()) {
        case tok::kw_bool:
            type = PrimativeType::getBool();
            break;

        case tok::kw_void:
            type = PrimativeType::getVoid();
            break;

        case tok::kw_char:
        case tok::kw_int8:
            type = PrimativeType::getInt8();
            break;

        case tok::kw_short:
        case tok::kw_int16:
            type = PrimativeType::getInt16();
            break;

        case tok::kw_int:
        case tok::kw_int32:
            type = PrimativeType::getInt32();
            break;

        case tok::kw_long:
        case tok::kw_int64:
            type = PrimativeType::getInt64();
            break;

        case tok::kw_uchar:
        case tok::kw_uint8:
            type = PrimativeType::getUInt8();
            break;

        case tok::kw_ushort:
        case tok::kw_uint16:
            type = PrimativeType::getUInt16();
            break;

        case tok::kw_uint:
        case tok::kw_uint32:
            type = PrimativeType::getUInt32();
            break;

        case tok::kw_ulong:
        case tok::kw_uint64:
            type = PrimativeType::getUInt64();
            break;

        case tok::kw_float:
        case tok::kw_float32:
            type = PrimativeType::getFloat32();
            break;

        case tok::kw_double:
        case tok::kw_float64:
            type = PrimativeType::getFloat64();
            break;

        default:
            throw new ParseException(peekTok().getSourceLocation(), "Unknown primative type");
    }

    return type;
}
