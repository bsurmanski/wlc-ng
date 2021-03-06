#include "parser.hpp"

#include "program.hpp"
#include "exception/exception.hpp"
#include "exception/parseException.hpp"
#include "ast/module.hpp"
#include "lex/lexer.hpp"

#include <stdio.h>
#include <assert.h>

Parser::Parser(Program *_program, Lexer *_lex) : error(0), program(_program), lex(_lex), module(NULL) {
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

int Parser::ignoreNewlines() {
    int c = 0;
    while(peekTok().getKind() == tok::newline) {
        c++;
        ignoreTok();
    }
    return c;
}

int Parser::ignoreStmtTerminators() {
    int c = 0;
    while(peekTok().isTerminator()) {
        c++;
        ignoreTok();
    }
    return c;
}

Token Parser::getTok() {
    if(!tbuf.empty()) {
        return tbuf.pop();
    } else {
        return lex->lex();
    }
}

void Parser::emit_error(SourceLocation loc, String err) {
    String msg;
    msg.append(loc.toString());
    msg.append(": ");
    msg.append(err);
    printf("%s\n", msg.c_str());
    error++;
}

/*
 * Stmt
 */

Stmt *Parser::parseStmt() {
    Token &token = peekTok();

    Stmt *stmt = NULL;

    if(token.isTypeKeyword() || token.isIdTypeKeyword() || token.isModifierKeyword() ||
            (token.isIdentifier() && peekTok(1).isIdentifier())) {
        stmt = parseDecl();
    } else {
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

            case tok::kw_for:
                stmt = parseForStmt();
                break;

            //case tok::kw_foreach:
            //    stmt = parseForEachStmt();
            //    break;

            case tok::kw_while:
                stmt = parseWhileStmt();
                break;

            case tok::kw_do:
                stmt = parseDoWhileStmt();
                break;

            //TODO: use_stmt, import_stmt

            default:
                // parse Expr, and get an assignment postfix if appropriate
                stmt = parseAssignPostfix(parseExpr());
                break;
        }
    }

    /*
    do {
        if(!peekTok().isTerminator() && !peekTok().is(tok::eof)) {
            throw ParseException(peekTok().getSourceLocation(), String("Expected terminator following statement. Found: ") + peekTok().getStringRepr());
        }
        ignoreTok();
    } while(peekTok().isTerminator());
    */

    //TODO: stmt terminator
    return stmt;
}

Stmt *Parser::parseAssignPostfix(Expr *lhs) {
    Stmt *stmt = lhs;
    switch(peekTok().getKind()) {
        case tok::equal:
            ignoreTok(); // ignore '='
            ignoreNewlines();
            stmt = new AssignStmt(lhs, parseExpr());
            break;
        case tok::plusequal:
        case tok::minusequal:
        case tok::starequal:
        case tok::slashequal:
        case tok::ampequal:
        case tok::barequal:
        case tok::caretequal:
        case tok::percentequal:
            throw ParseException(peekTok().getSourceLocation(), "unimplemented: augmented assignment");
        default:
            //XXX: := ?
            //TODO: augmented assign, eg: +=, -=, *=
            //throw ParseException(peekTok().getSourceLocation(), String("unknown operator: ") + peekTok().getStringRepr());
            break;
    }
    return stmt;
}

CompoundStmt *Parser::parseCompoundStmt() {
    DynArray<Stmt*> stmts;

    if(peekTok().getKind() != tok::lbrace || !ignoreTok()) {
        throw ParseException(peekTok().getSourceLocation(), "expected '{' beginning compound stmt");
    }

    ignoreNewlines();

    if(peekTok().getKind() != tok::rbrace) {
        stmts.append(parseStmt());
        while(ignoreStmtTerminators() && peekTok().getKind() != tok::rbrace) {
            stmts.append(parseStmt());
        }
    }

    if(peekTok().getKind() != tok::rbrace || !ignoreTok()) {
        throw ParseException(peekTok().getSourceLocation(), "expected '}' following compound stmt");
    }

    return new CompoundStmt(stmts);
}

LabelStmt *Parser::parseLabelStmt() {
    if(peekTok().getKind() != tok::kw_label || !ignoreTok()) {
        throw ParseException(peekTok().getSourceLocation(), "expected 'label' beginning label stmt");
    }

    Token tok = getTok();
    if(tok.getKind() != tok::identifier) {
        throw ParseException(tok.getSourceLocation(), "expected identifier following 'label' keyword");
    }

    return new LabelStmt(tok.getIdentifierName());
}

GotoStmt *Parser::parseGotoStmt() {
    if(peekTok().getKind() != tok::kw_goto || !ignoreTok()) {
        throw ParseException(peekTok().getSourceLocation(), "expected 'goto' beginning goto stmt");
    }

    Token tok = getTok();
    if(tok.getKind() != tok::identifier) {
        throw ParseException(tok.getSourceLocation(), "expected identifier following 'goto' keyword");
    }

    return new GotoStmt(tok.getIdentifierName());
}

BreakStmt *Parser::parseBreakStmt() {
    if(peekTok().getKind() != tok::kw_break || !ignoreTok()) {
        throw ParseException(peekTok().getSourceLocation(), "expected 'break' beginning break stmt");
    }

    return new BreakStmt;
}

ContinueStmt *Parser::parseContinueStmt() {
    if(peekTok().getKind() != tok::kw_continue || !ignoreTok()) {
        throw ParseException(peekTok().getSourceLocation(), "expected 'continue' beginning continue stmt");
    }

    return new ContinueStmt;
}

ReturnStmt *Parser::parseReturnStmt() {
    if(peekTok().getKind() != tok::kw_return || !ignoreTok()) {
        throw ParseException(peekTok().getSourceLocation(), "expected 'return' keyword");
    }

    Expr *expr = NULL;
    if(!peekTok().isTerminator()) {
        expr = parseExpr();
    }

    return new ReturnStmt(expr);
}

CaseStmt *Parser::parseCaseStmt() {
    throw ParseException(peekTok().getSourceLocation(), "unimplemented: parse case stmt");
}

IfStmt *Parser::parseIfStmt() {
    if(peekTok().getKind() != tok::kw_if || !ignoreTok()) {
        throw ParseException(peekTok().getSourceLocation(), "expected 'if' keyword beginning if stmt");
    }

    if(peekTok().getKind() != tok::lparen || !ignoreTok()) {
        throw ParseException(peekTok().getSourceLocation(), "expected '(' in if stmt");
    }

    ignoreNewlines();

    Expr *cond_expr = parseExpr();

    ignoreNewlines();

    if(peekTok().getKind() != tok::rparen || !ignoreTok()) {
        throw ParseException(peekTok().getSourceLocation(), String("expected ')' in if stmt. Found: ") + peekTok().getStringRepr());
    }

    Stmt *body = parseStmt();
    Stmt *elseBody = NULL;

    if(peekTok().getKind() == tok::kw_else) {
        ignoreTok();
        elseBody = parseStmt();
    }

    return new IfStmt(cond_expr, body, elseBody);
}

WhileStmt *Parser::parseWhileStmt() {
    if(peekTok().getKind() != tok::kw_while || !ignoreTok()) {
        throw ParseException(peekTok().getSourceLocation(), "expected 'while' beginning loop");
    }

    if(peekTok().getKind() != tok::lparen || !ignoreTok()) {
        throw ParseException(peekTok().getSourceLocation(), "expected '(' in while stmt");
    }

    ignoreNewlines();

    Expr *cond = parseExpr();

    ignoreNewlines();

    if(peekTok().getKind() != tok::rparen || !ignoreTok()) {
        throw ParseException(peekTok().getSourceLocation(), "expected ')' in while stmt");
    }

    Stmt *body = parseStmt();
    Stmt *elseBody = NULL;

    if(peekTok().getKind() == tok::kw_else) {
        ignoreTok();
        elseBody = parseStmt();
    }

    return new WhileStmt(cond, body, elseBody);
}

DoWhileStmt *Parser::parseDoWhileStmt() {
    if(peekTok().getKind() != tok::kw_do || !ignoreTok()) {
        throw ParseException(peekTok().getSourceLocation(), "expected 'do' beginning loop");
    }

    ignoreNewlines();

    Stmt *body = parseStmt();

    ignoreNewlines();

    if(peekTok().getKind() != tok::kw_while || !ignoreTok()) {
        throw ParseException(peekTok().getSourceLocation(), "expected 'while' following do");
    }

    if(peekTok().getKind() != tok::lparen || !ignoreTok()) {
        throw ParseException(peekTok().getSourceLocation(), "expected '(' in while stmt");
    }

    ignoreNewlines();

    Expr *cond = parseExpr();

    ignoreNewlines();

    if(peekTok().getKind() != tok::rparen || !ignoreTok()) {
        throw ParseException(peekTok().getSourceLocation(), "expected ')' in while stmt");
    }

    Stmt *elseBody = NULL;
    if(peekTok().getKind() == tok::kw_else) {
        ignoreTok();
        elseBody = parseStmt();
    }

    return new DoWhileStmt(cond, body, elseBody);
}

ForStmt *Parser::parseForStmt() {
    Decl *decl = NULL;
    Expr *cond = NULL;
    Stmt *upd = NULL;
    Stmt *body = NULL;
    Stmt *elseBody = NULL;

    if(peekTok().getKind() != tok::kw_for || !ignoreTok()) {
        throw ParseException(peekTok().getSourceLocation(), "expected 'for' beginning loop");
    }

    if(peekTok().getKind() != tok::lparen || !ignoreTok()) {
        throw ParseException(peekTok().getSourceLocation(), "expected '(' in while stmt");
    }

    ignoreNewlines();
    decl = parseDecl();

    if(peekTok().getKind() != tok::semicolon || !ignoreTok()) {
        throw ParseException(peekTok().getSourceLocation(), "expected ';' following decl in for stmt");
    }

    ignoreNewlines();
    cond = parseExpr();

    if(peekTok().getKind() != tok::semicolon || !ignoreTok()) {
        throw ParseException(peekTok().getSourceLocation(), "expected ';' following condition in for stmt");
    }

    ignoreNewlines();
    upd = parseStmt();
    ignoreNewlines();

    if(peekTok().getKind() != tok::rparen || !ignoreTok()) {
        throw ParseException(peekTok().getSourceLocation(), "expected ')' following decl in for stmt");
    }

    ignoreNewlines();
    body = parseStmt();

    if(peekTok().getKind() == tok::kw_else) {
        ignoreTok();
        elseBody = parseStmt();
    }

    //TODO: create for stmt
    throw ParseException(peekTok().getSourceLocation(), "unimplemented: parse for");
}

ForEachStmt *Parser::parseForEachStmt() {
    /*
    if(peekTok().getKind() != tok::kw_foreach || !ignoreTok()) {
        throw ParseException(peekTok().getSourceLocation(), "expected 'foreach' beginning loop");
    }*/

    throw ParseException(peekTok().getSourceLocation(), "unimplemented: foreach");
}

/*
 * Expr
 */
Expr *Parser::parseExpr() {
    return parsePostfixExpr(parseBinaryExpr());
}

DynArray<Expr*> Parser::parseExprList() {
    DynArray<Expr*> exprs;
    exprs.append(parseExpr());
     while(peekTok().getKind() == tok::comma && ignoreTok()) {
        ignoreNewlines();
        exprs.append(parseExpr());
    }
    return exprs;
}

CallExpr *Parser::parseCallExprPostfix(Expr *expr) {
    DynArray<Expr *> args;

    if(peekTok().getKind() != tok::lparen) {
        throw ParseException(peekTok().getSourceLocation(), "expected '(' beginning call postfix");
    }

    ignoreTok();
    ignoreNewlines();
    if(peekTok().getKind() != tok::rparen) {
        args = parseExprList();
        if(peekTok().getKind() != tok::rparen) {
            throw ParseException(peekTok().getSourceLocation(), "expected ')' following call expr");
        }
    }
    ignoreTok();

    return new CallExpr(expr, args);
}

Expr *Parser::parsePostfixExpr(Expr *expr) {
    while(true) {
        switch(peekTok().getKind()) {
            case tok::plusplus:
                ignoreTok();
                expr = new PostIncExpr(expr);
                break;
            case tok::minusminus:
                ignoreTok();
                expr = new PostDecExpr(expr);
                break;
            case tok::lparen:
                expr = parseCallExprPostfix(expr);
                break;
            case tok::lbracket:
                ignoreTok();
                ignoreNewlines();
                expr = new IndexExpr(expr, parseExpr());
                ignoreNewlines();
                if(peekTok().getKind() != tok::rbracket || !ignoreTok()) {
                    throw ParseException(peekTok().getSourceLocation(), "expected ']' following index postfix");
                }
                break;
            case tok::dot:
                ignoreTok();
                if(peekTok().getKind() != tok::identifier) {
                    throw ParseException(peekTok().getSourceLocation(), String("expected identifier following '.' found: ") + peekTok().getStringRepr());
                }
                expr = new MemberExpr(expr, getTok().getIdentifierName());
                break;
            default:
                return expr;
        }
    }
}

Expr *Parser::parsePrimaryExpr() {
    Token tok = getTok();
    Expr *expr;
    switch(tok.getKind()) {
        case tok::lbracket:
            throw ParseException(peekTok().getSourceLocation(), "unimplemented: parse tuple expr");

        case tok::lparen:
            ignoreNewlines();
            expr = parseExpr();
            ignoreNewlines();
            if(peekTok().getKind() != tok::rparen || !ignoreTok()) {
                throw ParseException(peekTok().getSourceLocation(), "expected )");
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
            return new FloatLiteralExpr(tok.getFloatData());

        case tok::intlit:
            return new IntLiteralExpr(tok.getIntData());

        case tok::stringlit:
            return new StringLiteralExpr(tok.getStringData());

        case tok::charlit:
            throw ParseException(peekTok().getSourceLocation(), String("invalid or unimplemented primary expr: ") + tok.getStringRepr());

        case tok::kw_new:
            return new NewExpr(parseType());

        case tok::kw_delete:
            return new DeleteExpr(parseExpr());

        case tok::kw_retain:
            return new RetainExpr(parseExpr());

        case tok::kw_release:
            return new ReleaseExpr(parseExpr());

        case tok::kw_pack:
            if(peekTok().getKind() != tok::stringlit) {
                throw ParseException(peekTok().getSourceLocation(), String("expected string following 'pack' keyword"));
            }
            return new PackExpr(getTok().getStringData());

        default:
            throw ParseException(peekTok().getSourceLocation(), String("invalid or unimplemented primary expr: ") + tok.getStringRepr());
    }
}

Expr *Parser::parseBinaryExpr(int precidence) {
    Expr *lhs = parseUnaryExpr();

    Token tok = peekTok();
    while(tok.mayBeBinaryOperator() && tok.binaryOperatorPrecidence() > precidence) {
        ignoreTok();
        switch(tok.getKind()) {
            case tok::plus:
                ignoreNewlines();
                lhs = new AddExpr(lhs, parseBinaryExpr(tok.binaryOperatorPrecidence()));
                break;

            case tok::minus:
                ignoreNewlines();
                lhs = new SubExpr(lhs, parseBinaryExpr(tok.binaryOperatorPrecidence()));
                break;

            case tok::star:
                ignoreNewlines();
                lhs = new MulExpr(lhs, parseBinaryExpr(tok.binaryOperatorPrecidence()));
                break;

            case tok::starstar:
                ignoreNewlines();
                lhs = new PowerExpr(lhs, parseBinaryExpr(tok.binaryOperatorPrecidence()));
                break;

            case tok::slash:
                ignoreNewlines();
                lhs = new DivExpr(lhs, parseBinaryExpr(tok.binaryOperatorPrecidence()));
                break;

            case tok::amp:
                ignoreNewlines();
                lhs = new BitAndExpr(lhs, parseBinaryExpr(tok.binaryOperatorPrecidence()));
                break;

            case tok::ampamp:
                ignoreNewlines();
                lhs = new AndExpr(lhs, parseBinaryExpr(tok.binaryOperatorPrecidence()));
                break;

            case tok::bar:
                ignoreNewlines();
                lhs = new BitOrExpr(lhs, parseBinaryExpr(tok.binaryOperatorPrecidence()));
                break;

            case tok::barbar:
                ignoreNewlines();
                lhs = new OrExpr(lhs, parseBinaryExpr(tok.binaryOperatorPrecidence()));
                break;

            case tok::caret:
                ignoreNewlines();
                lhs = new BitXorExpr(lhs, parseBinaryExpr(tok.binaryOperatorPrecidence()));
                break;

            case tok::percent:
                ignoreNewlines();
                lhs = new ModulusExpr(lhs, parseBinaryExpr(tok.binaryOperatorPrecidence()));
                break;

            case tok::lessless:
                ignoreNewlines();
                lhs = new LShiftExpr(lhs, parseBinaryExpr(tok.binaryOperatorPrecidence()));
                break;

            case tok::greatergreater:
                ignoreNewlines();
                lhs = new RShiftExpr(lhs, parseBinaryExpr(tok.binaryOperatorPrecidence()));
                break;
            case tok::less:
                ignoreNewlines();
                lhs = new LessThanExpr(lhs, parseBinaryExpr(tok.binaryOperatorPrecidence()));
                break;
            case tok::lessequal:
                ignoreNewlines();
                lhs = new LessOrEqualExpr(lhs, parseBinaryExpr(tok.binaryOperatorPrecidence()));
                break;
            case tok::greater:
                ignoreNewlines();
                lhs = new GreaterThanExpr(lhs, parseBinaryExpr(tok.binaryOperatorPrecidence()));
                break;
            case tok::greaterequal:
                ignoreNewlines();
                lhs = new GreaterOrEqualExpr(lhs, parseBinaryExpr(tok.binaryOperatorPrecidence()));
                break;
            case tok::bangequal:
                ignoreNewlines();
                lhs = new NotEqualExpr(lhs, parseBinaryExpr(tok.binaryOperatorPrecidence()));
                break;
            case tok::equal:
                ignoreNewlines();
                lhs = new EqualExpr(lhs, parseBinaryExpr(tok.binaryOperatorPrecidence()));
                break;
            default:
                break;
        }

        tok = peekTok();
    }

    return lhs;
}

Expr *Parser::parseUnaryExpr() {
    Token op = peekTok();
    switch(op.getKind()) {
        case tok::plus:
            ignoreTok();
            throw ParseException(peekTok().getSourceLocation(), "unimplemented: parse unary +");
        case tok::plusplus:
            ignoreTok();
            return new PreIncExpr(parseUnaryExpr());
        case tok::minus:
            ignoreTok();
            return new NegateExpr(parseUnaryExpr());
        case tok::minusminus:
            ignoreTok();
            return new PreDecExpr(parseUnaryExpr());
        case tok::amp:
            ignoreTok();
            return new RefExpr(parseUnaryExpr());
        case tok::caret:
            ignoreTok();
            return new DerefExpr(parseUnaryExpr());
        case tok::bang:
            ignoreTok();
            return new NotExpr(parseUnaryExpr());
        case tok::tilde:
            ignoreTok();
            throw ParseException(peekTok().getSourceLocation(), "unimplemented: parse unary ~");
        default:
            return parsePrimaryExpr(); //XXX parse postfix
    }
}

/*
 * Decl
 */
Decl *Parser::parseDecl() {
    //TODO: parse access modifier

    switch(peekTok().getKind()) {
        case tok::kw_struct:
        case tok::kw_class:
        case tok::kw_union:
        case tok::kw_interface:
            return parseTypeDecl();
        default:
            return parseNonTypeDecl();
    }
    throw ParseException(peekTok().getSourceLocation(), "unimplemented: parse decl");
}

TypeDecl *Parser::parseTypeDecl() {
    IdTypeDecl *tdecl = NULL;
    switch(peekTok().getKind()) {
        case tok::kw_struct:
            tdecl = new StructDecl;
            break;
        case tok::kw_class:
            tdecl = new ClassDecl;
            break;
        case tok::kw_union:
            tdecl = new UnionDecl;
            break;
        case tok::kw_interface:
            tdecl = new InterfaceDecl;
            break;
        default:
            throw ParseException(peekTok().getSourceLocation(), "exepected type declaration kind");
    }
    ignoreTok();

    ignoreNewlines();
    if(peekTok().getKind() == tok::identifier) {
        tdecl->setName(getTok().getIdentifierName());
    }

    ignoreNewlines();
    if(peekTok().getKind() == tok::colon) {
        if(!tdecl->isClassDecl()) {
            emit_error(peekTok().getSourceLocation(), "base class specifier on non-class type");
        }

        ignoreTok();
        ignoreNewlines();
        if(peekTok().getKind() == tok::identifier) {
            tdecl->setBase(getTok().getIdentifierName());
        } else {
            emit_error(peekTok().getSourceLocation(), String("expected base class specifier following ':'. Found: ") + peekTok().getStringRepr());
            // TODO: error, expected base class identifier following colon
        }
    }

    ignoreNewlines();
    if(peekTok().getKind() != tok::lbrace || !ignoreTok()) {
        //TODO: error, expected opening brace following type declaration specifier
    }

    ignoreNewlines();
    while(peekTok().getKind() != tok::rbrace) {
        tdecl->addMember(parseDecl());

        if(getLexer()->eof()) {
            throw ParseException(peekTok().getSourceLocation(), "encountered EOF while parsing type declaration");
        }
        ignoreNewlines();
    }
    //TODO: add members to tdecl

    if(peekTok().getKind() != tok::rbrace || !ignoreTok()) {
            throw ParseException(peekTok().getSourceLocation(), "expected } following usertype declaration");
    }

    return tdecl;
}

DynArray<VarDecl*> Parser::parseParams() {
    if(peekTok().getKind() != tok::lparen || !ignoreTok()) {
        throw ParseException(peekTok().getSourceLocation(), "expected ( in function parameter specification");
    }

    DynArray<VarDecl*> params;
    do {
        ignoreNewlines();
        if(peekTok().getKind() == tok::rparen) {
            break;
        }

        Type *type = parseType();
        String name;
        Expr *value = NULL;

        if(peekTok().getKind() != tok::identifier) {
            //TODO: destroy params?
            throw ParseException(peekTok().getSourceLocation(), "expected identifier in parameter list");
        }

        name = getTok().getIdentifierName();

        if(peekTok().getKind() == tok::equal) {
            ignoreTok();
            ignoreNewlines();
            value = parseExpr(); //TODO: try catch
        }
        params.append(new VarDecl(type, name, value));
    } while(peekTok().getKind() == tok::comma && ignoreTok());

    if(peekTok().getKind() != tok::rparen || !ignoreTok()) {
        //TODO: destroy params?
        throw ParseException(peekTok().getSourceLocation(), "expected ) following function parameter specifier");
    }

    return params;
}

Decl *Parser::parseNonTypeDecl() {
    Type *type = parseType();
    String name;

    if(peekTok().getKind() != tok::identifier) {
        throw ParseException(peekTok().getSourceLocation(), "expected identifier following type");
    }

    name = getTok().getIdentifierName();

    // function decl
    if(peekTok().getKind() == tok::lparen) {
        DynArray<VarDecl*> params = parseParams();
        Stmt *body = parseStmt(); //TODO: allow empty stmt
        return new FuncDecl(type, name, params, body);
    } else {
        Expr *defaultValue = NULL;
        if(peekTok().getKind() == tok::equal) {
            ignoreTok();
            defaultValue = parseExpr();
        }
        return new VarDecl(type, name, defaultValue);
    }
}

/*
 * Type
 */

Type *Parser::parseType() {
    Type *type = NULL;
    switch(peekTok().getKind()) {
        case tok::lbracket:
            type = parseTupleType();
            break;

        case tok::identifier:
            return new IdType(getTok().getStringData());
            break;

        case tok::kw_bool:
        case tok::kw_void:
        case tok::kw_char:
        case tok::kw_int8:
        case tok::kw_short:
        case tok::kw_int16:
        case tok::kw_int:
        case tok::kw_int32:
        case tok::kw_long:
        case tok::kw_int64:
        case tok::kw_uchar:
        case tok::kw_uint8:
        case tok::kw_ushort:
        case tok::kw_uint16:
        case tok::kw_uint:
        case tok::kw_uint32:
        case tok::kw_ulong:
        case tok::kw_uint64:
        case tok::kw_float:
        case tok::kw_float32:
        case tok::kw_double:
        case tok::kw_float64:
            type = parsePrimativeType();
            break;
        default:
        throw ParseException(peekTok().getSourceLocation(), String("could not parse type. On token: ") + peekTok().getStringRepr());
    }

    return parseTypePostfix(type);
}

Type *Parser::parseTypePostfix(Type *base) {
    Type *type = base;

    switch(peekTok().getKind()) {
        case tok::caret:
            ignoreTok();
            type = base->getPointer();
            break;

        //TODO: function, array
        default:
        return base;
    }

    return parseTypePostfix(type); // can have multiple postfixes
}

TupleType *Parser::parseTupleType() {
    ignoreTok();
    DynArray<Type*> types; //TODO: create linked list instead? type = type.tupledWith(parseType())
    Type *ty = NULL;
    while(true) {
        types.append(parseType());
        if(peekTok().getKind() != tok::comma) {
            break;
        } else {
            ignoreTok();
        }
    }

    if(peekTok().getKind() != tok::rbracket || ignoreTok()) {
        throw ParseException(peekTok().getSourceLocation(), "expected ] following tuple type list");
    }

    //TODO: get tuple type
    throw ParseException(peekTok().getSourceLocation(), "unimplemented: tuple type");
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
            throw ParseException(peekTok().getSourceLocation(), "Unknown primative type");
    }

    ignoreTok();
    return type;
}
