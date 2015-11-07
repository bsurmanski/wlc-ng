#ifndef _EXPR_HPP
#define _EXPR_HPP

#include "stmt.hpp"
#include "type.hpp"
#include "common/string.hpp"

class BinaryExpr;
class UnaryExpr;
class LiteralExpr;
class CharLiteralExpr;
class StringLiteralExpr;
class BoolLiteralExpr;
class NullLiteralExpr;
class NumericLiteralExpr;
class TupleExpr;
class MemExpr;
class NewExpr;
class DeleteExpr;
class RetainExpr;
class ReleaseExpr;
class IdExpr;
class PackExpr;
class MemberExpr;
class CallExpr;
class IndexExpr;
class CastExpr;

class Expr : public Stmt {
    public:
    virtual BinaryExpr          *asBinaryExpr();
    virtual UnaryExpr           *asUnaryExpr();
    virtual LiteralExpr         *asLiteralExpr();
    virtual CharLiteralExpr     *asCharLiteralExpr();
    virtual StringLiteralExpr   *asStringLiteralExpr();
    virtual BoolLiteralExpr     *asBoolLiteralExpr();
    virtual NullLiteralExpr     *asNullLiteralExpr();
    virtual NumericLiteralExpr  *asNumericLiteralExpr();
    virtual TupleExpr           *asTupleExpr();
    virtual MemExpr             *asMemExpr();
    virtual NewExpr             *asNewExpr();
    virtual DeleteExpr          *asDeleteExpr();
    virtual RetainExpr          *asRetainExpr();
    virtual ReleaseExpr         *asReleaseExpr();
    virtual IdExpr              *asIdExpr();
    virtual PackExpr            *asPackExpr();
    virtual MemberExpr          *asMemberExpr();
    virtual CallExpr            *asCallExpr();
    virtual IndexExpr           *asIndexExpr();
    virtual CastExpr            *asCastExpr();

    virtual bool isBinaryExpr();
    virtual bool isUnaryExpr();
    virtual bool isLiteralExpr();
    virtual bool isCharLiteralExpr();
    virtual bool isStringLiteralExpr();
    virtual bool isBoolLiteralExpr();
    virtual bool isNullLiteralExpr();
    virtual bool isNumericLiteralExpr();
    virtual bool isTupleExpr();
    virtual bool isMemExpr();
    virtual bool isNewExpr();
    virtual bool isDeleteExpr();
    virtual bool isRetainExpr();
    virtual bool isReleaseExpr();
    virtual bool isIdExpr();
    virtual bool isPackExpr();
    virtual bool isMemberExpr();
    virtual bool isCallExpr();
    virtual bool isIndexExpr();
    virtual bool isCastExpr();

    virtual Type *getType() {return NULL;} //= 0; //TODO pure virtual
};

class BinaryExpr : public Expr {
    Expr *lhs;
    Expr *rhs;

    public:
    virtual BinaryExpr *asBinaryExpr();
};

class UnaryExpr : public Expr {
    Expr *operand;

    public:
    UnaryExpr(Expr *_operand);
    virtual UnaryExpr *asUnaryExpr();
    virtual Type *getType(); //TODO
};

class PreIncExpr : public UnaryExpr {
    public:
    PreIncExpr(Expr *_operand);
};

class PreDecExpr : public UnaryExpr {
    public:
    PreDecExpr(Expr *_operand);
};

class NegateExpr : public UnaryExpr {
    public:
    NegateExpr(Expr *_operand);
};

class NotExpr : public UnaryExpr {
    public:
    NotExpr(Expr *_operand);
};

class DerefExpr : public UnaryExpr {
    public:
    DerefExpr(Expr *_operand);
};

class RefExpr : public UnaryExpr {
    public:
    RefExpr(Expr *_operand);
};

class LiteralExpr : public Expr {
    public:
    virtual LiteralExpr *asLiteralExpr();
};

class CharLiteralExpr : public LiteralExpr {
    char value;

    public:
    virtual CharLiteralExpr *asCharLiteralExpr();
};

class StringLiteralExpr : public LiteralExpr {
    String value;

    public:
    virtual StringLiteralExpr *asStringLiteralExpr();
};

class BoolLiteralExpr : public LiteralExpr {
    bool value;

    public:
    BoolLiteralExpr(bool _value);
    virtual BoolLiteralExpr *asBoolLiteralExpr();
};

class NullLiteralExpr : public LiteralExpr {
    public:
    virtual NullLiteralExpr *asNullLiteralExpr();
};

class NumericLiteralExpr : public LiteralExpr {
    public:
    virtual NumericLiteralExpr *asNumericLiteralExpr();
};

class TupleExpr : public Expr {
    public:
    virtual TupleExpr *asTupleExpr();
};

class MemExpr : public Expr {
    public:
    virtual MemExpr *asMemExpr();
};

class NewExpr : public MemExpr {
    public:
    virtual NewExpr *asNewExpr();
};

class DeleteExpr : public MemExpr {
    Expr *operand;

    public:
    virtual DeleteExpr *asDeleteExpr();
};

class RetainExpr : public MemExpr {
    Expr *operand;

    public:
    virtual RetainExpr *asRetainExpr();
};

class ReleaseExpr : public MemExpr {
    Expr *operand;

    public:
    virtual ReleaseExpr *asReleaseExpr();
};

class IdExpr : public Expr {
    String name;

    public:
    virtual IdExpr *asIdExpr();
};

class PackExpr : public Expr {
    String filename;

    public:
    virtual PackExpr *asPackExpr();
};

class MemberExpr : public Expr {
    public:
    virtual MemberExpr *asMemberExpr();
};

class CallExpr : public Expr {
    public:
    virtual CallExpr *asCallExpr();
};

class IndexExpr : public Expr {
    public:
    virtual IndexExpr *asIndexExpr();
};

class CastExpr : public Expr {
    Expr *operand;

    public:
    virtual CastExpr *asCastExpr();
};

#endif
