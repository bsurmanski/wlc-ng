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
            throw ParseException(peekTok().getSourceLocation(), String("Expected line-terminator"));
        }
    } while(peekTok().isTerminator());

    //TODO: stmt terminator
    return stmt;
}

CompoundStmt *Parser::parseCompoundStmt() {
    if(peekTok().getKind() != tok::lbrace || !ignoreTok()) {
        throw ParseException(peekTok().getSourceLocation(), "expected left brace beginning compound stmt");
    }

    DynArray<Stmt*> stmts;
    while(peekTok().getKind() != tok::rbrace) {
        Stmt *stmt = parseStmt();
        stmts.append(stmt);
    }

    if(peekTok().getKind() != tok::lbrace || !ignoreTok()) {
        throw ParseException(peekTok().getSourceLocation(), "expected right brace following compound stmt");
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

    //TODO: line terminator

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

    //TODO: line terminator

    return new GotoStmt(tok.getIdentifierName());
}

BreakStmt *Parser::parseBreakStmt() {
    if(peekTok().getKind() != tok::kw_break || !ignoreTok()) {
        throw ParseException(peekTok().getSourceLocation(), "expected 'break' beginning break stmt");
    }

    //TODO: line terminator

    return new BreakStmt;
}

ContinueStmt *Parser::parseContinueStmt() {
    if(peekTok().getKind() != tok::kw_continue || !ignoreTok()) {
        throw ParseException(peekTok().getSourceLocation(), "expected 'continue' beginning continue stmt");
    }

    //TODO: line terminator

    return new ContinueStmt;
}

ReturnStmt *Parser::parseReturnStmt() {
    if(peekTok().getKind() != tok::kw_return || !ignoreTok()) {
        throw ParseException(peekTok().getSourceLocation(), "expected 'return' keyword");
    }

    //TODO: check line terminator before trying to read value
    Expr *expr = parseExpr();
    //TODO: line termnator
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

    Expr *cond_expr = parseExpr();

    if(peekTok().getKind() != tok::rparen || !ignoreTok()) {
        throw ParseException(peekTok().getSourceLocation(), "expected ')' in if stmt");
    }

    Stmt *cond_stmt = parseStmt();
    Stmt *else_stmt = NULL;

    if(peekTok().getKind() == tok::kw_else) {
        ignoreTok();
        else_stmt = parseStmt();
    }

    //TODO: return new IfStmt(cond_expr, cond_stmt, else_stmt);

    throw ParseException(peekTok().getSourceLocation(), "unimplemented: parse if stmt");
}

/*
 * Expr
 */
Expr *Parser::parseExpr() {
    switch(peekTok().getKind()) {
        case tok::kw_struct:
        case tok::kw_class:
        case tok::kw_union:
        case tok::kw_interface:
            return parseDecl();
        case tok::identifier:
            if(peekTok(1).getKind() == tok::identifier) {
                return parseDecl();
            } // else fall through
        default:
            return parseBinaryExpr();
    }
}

Expr *Parser::parsePrimaryExpr() {
    Token tok = getTok();
    Expr *expr;
    switch(tok.getKind()) {
        case tok::lbracket:
            throw ParseException(peekTok().getSourceLocation(), "unimplemented: parse tuple expr");

        case tok::lparen:
            expr = parseExpr();
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
                lhs = new AddExpr(lhs, parseBinaryExpr(tok.binaryOperatorPrecidence()));
                break;

            case tok::minus:
                lhs = new SubExpr(lhs, parseBinaryExpr(tok.binaryOperatorPrecidence()));
                break;

            case tok::star:
                lhs = new MulExpr(lhs, parseBinaryExpr(tok.binaryOperatorPrecidence()));
                break;

            case tok::starstar:
                lhs = new PowerExpr(lhs, parseBinaryExpr(tok.binaryOperatorPrecidence()));
                break;

            case tok::slash:
                lhs = new DivExpr(lhs, parseBinaryExpr(tok.binaryOperatorPrecidence()));
                break;

            case tok::amp:
                lhs = new BitAndExpr(lhs, parseBinaryExpr(tok.binaryOperatorPrecidence()));
                break;

            case tok::ampamp:
                lhs = new AndExpr(lhs, parseBinaryExpr(tok.binaryOperatorPrecidence()));
                break;

            case tok::bar:
                lhs = new BitOrExpr(lhs, parseBinaryExpr(tok.binaryOperatorPrecidence()));
                break;

            case tok::barbar:
                lhs = new OrExpr(lhs, parseBinaryExpr(tok.binaryOperatorPrecidence()));
                break;

            case tok::caret:
                lhs = new BitXorExpr(lhs, parseBinaryExpr(tok.binaryOperatorPrecidence()));
                break;

            case tok::percent:
                lhs = new ModulusExpr(lhs, parseBinaryExpr(tok.binaryOperatorPrecidence()));
                break;

            case tok::lessless:
                lhs = new LShiftExpr(lhs, parseBinaryExpr(tok.binaryOperatorPrecidence()));
                break;

            case tok::greatergreater:
                lhs = new RShiftExpr(lhs, parseBinaryExpr(tok.binaryOperatorPrecidence()));
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
    UserTypeDecl *tdecl = NULL;
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

    if(peekTok().getKind() == tok::identifier) {
        tdecl->setName(getTok().getIdentifierName());
    }

    if(peekTok().getKind() == tok::colon) {
        if(!tdecl->isClassDecl()) {
            emit_error(peekTok().getSourceLocation(), "base class specifier on non-class type");
        }

        ignoreTok();

        if(peekTok().getKind() == tok::identifier) {
            //TODO: tdecl->SetBaseClass(getTok().getIdentifierName());
        } else {
            // TODO: error, expected base class identifier following colon
        }
    }

    if(peekTok().getKind() != tok::lbrace) {
        //TODO: error, expected opening brace following type declaration specifier
    }

    DynArray<Decl*> members;
    while(peekTok().getKind() != tok::rbrace) {
        members.append(parseDecl());

        if(getLexer()->eof()) {
            throw ParseException(peekTok().getSourceLocation(), "encountered EOF while parsing type declaration");
        }
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
        //TODO: default value?
        //TODO: expect EOL
        return new VarDecl(type, name, NULL);
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
    }

    if(!type) {
        throw ParseException(peekTok().getSourceLocation(), "could not parse type");
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
