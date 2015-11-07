#include "expr.hpp"

BinaryExpr *Expr::asBinaryExpr() {
    return NULL;
}

UnaryExpr *Expr::asUnaryExpr() {
    return NULL;
}

LiteralExpr *Expr::asLiteralExpr() {
    return NULL;
}

CharLiteralExpr *Expr::asCharLiteralExpr() {
    return NULL;
}

StringLiteralExpr *Expr::asStringLiteralExpr() {
    return NULL;
}

BoolLiteralExpr *Expr::asBoolLiteralExpr() {
    return NULL;
}

NullLiteralExpr *Expr::asNullLiteralExpr() {
    return NULL;
}

NumericLiteralExpr *Expr::asNumericLiteralExpr() {
    return NULL;
}

TupleExpr *Expr::asTupleExpr() {
    return NULL;
}

MemExpr *Expr::asMemExpr() {
    return NULL;
}

NewExpr *Expr::asNewExpr() {
    return NULL;
}

DeleteExpr *Expr::asDeleteExpr() {
    return NULL;
}

RetainExpr *Expr::asRetainExpr() {
    return NULL;
}

ReleaseExpr *Expr::asReleaseExpr() {
    return NULL;
}

IdExpr *Expr::asIdExpr() {
    return NULL;
}

PackExpr *Expr::asPackExpr() {
    return NULL;
}

MemberExpr *Expr::asMemberExpr() {
    return NULL;
}

CallExpr *Expr::asCallExpr() {
    return NULL;
}

IndexExpr *Expr::asIndexExpr() {
    return NULL;
}

CastExpr *Expr::asCastExpr() {
    return NULL;
}

bool Expr::isBinaryExpr() {
    return (bool) asBinaryExpr();
}

bool Expr::isUnaryExpr() {
    return (bool) asUnaryExpr();
}

bool Expr::isLiteralExpr() {
    return (bool) asLiteralExpr();
}

bool Expr::isCharLiteralExpr() {
    return (bool) asCharLiteralExpr();
}

bool Expr::isStringLiteralExpr() {
    return (bool) asStringLiteralExpr();
}

bool Expr::isBoolLiteralExpr() {
    return (bool) asBoolLiteralExpr();
}

bool Expr::isNullLiteralExpr() {
    return (bool) asNullLiteralExpr();
}

bool Expr::isNumericLiteralExpr() {
    return (bool) asNumericLiteralExpr();
}

bool Expr::isTupleExpr() {
    return (bool) asTupleExpr();
}

bool Expr::isMemExpr() {
    return (bool) asMemExpr();
}

bool Expr::isNewExpr() {
    return (bool) asNewExpr();
}

bool Expr::isDeleteExpr() {
    return (bool) asDeleteExpr();
}

bool Expr::isRetainExpr() {
    return (bool) asRetainExpr();
}

bool Expr::isReleaseExpr() {
    return (bool) asReleaseExpr();
}

bool Expr::isIdExpr() {
    return (bool) asIdExpr();
}

bool Expr::isPackExpr() {
    return (bool) asPackExpr();
}

bool Expr::isMemberExpr() {
    return (bool) asMemberExpr();
}

bool Expr::isCallExpr() {
    return (bool) asCallExpr();
}

bool Expr::isIndexExpr() {
    return (bool) asIndexExpr();
}

bool Expr::isCastExpr() {
    return (bool) asCastExpr();
}

/*
 * UnaryExpr
 */

UnaryExpr::UnaryExpr(Expr *_operand) : operand(_operand) {
}

UnaryExpr *UnaryExpr::asUnaryExpr() {
    return this;
}

Type *UnaryExpr::getType() {
    return NULL; //TODO
}

PreIncExpr::PreIncExpr(Expr *_operand) : UnaryExpr(_operand) {
}

PreDecExpr::PreDecExpr(Expr *_operand) : UnaryExpr(_operand) {
}

NegateExpr::NegateExpr(Expr *_operand) : UnaryExpr(_operand) {
}

NotExpr::NotExpr(Expr *_operand) : UnaryExpr(_operand) {
}

DerefExpr::DerefExpr(Expr *_operand) : UnaryExpr(_operand) {
}

RefExpr::RefExpr(Expr *_operand) : UnaryExpr(_operand) {
}


/*
 * LiteralExpr
 */
LiteralExpr *LiteralExpr::asLiteralExpr() {
    return this;
}

BoolLiteralExpr *BoolLiteralExpr::asBoolLiteralExpr() {
    return this;
}

BoolLiteralExpr::BoolLiteralExpr(bool _value) : value(_value) {
}

NullLiteralExpr *NullLiteralExpr::asNullLiteralExpr() {
    return this;
}
