#ifndef _EXPR_HPP
#define _EXPR_HPP

#include "stmt.hpp"

class Expr : Stmt {
};

class BinaryExpr : Expr {
};

class UnaryExpr : Expr {
};

class LiteralExpr : Expr {
};

class CharLiteralExpr : LiteralExpr {
};

class StringLiteralExpr : LiteralExpr {
};

class BoolLiteralExpr : LiteralExpr {
};

class NullLiteralExpr : LiteralExpr {
};

class NumericLiteralExpr : LiteralExpr {
};

class TupleExpr : Expr {
};

class MemExpr : Expr {
};

class NewExpr : MemExpr {
};

class DeleteExpr : MemExpr {
};

class RetainExpr : MemExpr {
};

class ReleaseExpr : MemExpr {
};

class IdExpr : Expr {
}:

class PackExpr : Expr {
};

class MemberExpr : Expr {
};

class CallExpr : Expr {
};

class IndexExpr : Expr {
};

class CastExpr : Expr {
};

#endif
