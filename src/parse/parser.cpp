#include "parser.hpp"

#include "exception/exception.hpp"

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
            SourceLocation loc = peekTok().getSourceLocation();
            throw new Exception(String("Expected line-terminator at line ") + String::fromInt(loc.getLine()));
        }
    } while(peekTok().isTerminator());

    //TODO: stmt terminator
    return stmt;
}

CompoundStmt *Parser::parseCompoundStmt() {
    if(peekTok().getKind() != tok::lbrace || !ignoreTok()) {
        throw new Exception("expected left brace beginning compound stmt");
    }

    DynArray<Stmt*> stmts;
    while(peekTok().getKind() != tok::rbrace) {
        Stmt *stmt = parseStmt();
        stmts.append(stmt);
    }

    if(peekTok().getKind() != tok::lbrace || !ignoreTok()) {
        throw new Exception("expected right brace following compound stmt");
    }

    return new CompoundStmt(stmts);
}

LabelStmt *Parser::parseLabelStmt() {
    throw new Exception("unimplemented: parse label stmt");
}

GotoStmt *Parser::parseGotoStmt() {
    throw new Exception("unimplemented: parse goto stmt");
}

BreakStmt *Parser::parseBreakStmt() {
    throw new Exception("unimplemented: parse break stmt");
}

ContinueStmt *Parser::parseContinueStmt() {
    throw new Exception("unimplemented: parse continue stmt");
}

ReturnStmt *Parser::parseReturnStmt() {
    if(peekTok().getKind() != tok::kw_return) {
        throw new Exception("expected 'return' keyword");
    }
    ignoreTok();
    Expr *ex = parseExpr();
    //TODO
    throw new Exception("unimplemented: parse return stmt");
}

CaseStmt *Parser::parseCaseStmt() {
    throw new Exception("unimplemented: parse case stmt");
}

IfStmt *Parser::parseIfStmt() {
    throw new Exception("unimplemented: parse if stmt");
}

Expr *Parser::parseExpr() {
    throw new Exception("unimplemented: parse expr");
}

Decl *Parser::parseDecl() {
    throw new Exception("unimplemented: parse decl");
}

Type *Parser::parseType() {

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
            throw new Exception("Unknown primative type");
    }

    return type;
}
