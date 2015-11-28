#ifndef _EXPR_HPP
#define _EXPR_HPP

#include <stdint.h>

#include "stmt.hpp"
#include "type.hpp"
#include "common/string.hpp"
#include "common/stringFormatter.hpp"

class BinaryExpr;
class UnaryExpr;
class LiteralExpr;
class CharLiteralExpr;
class StringLiteralExpr;
class BoolLiteralExpr;
class NullLiteralExpr;
class NumericLiteralExpr;
class IntLiteralExpr;
class FloatLiteralExpr;
class TupleExpr;
class MemExpr;
class NewExpr;
class DeleteExpr;
class RetainExpr;
class ReleaseExpr;
class ThisExpr;
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
    virtual IntLiteralExpr      *asIntLiteralExpr();
    virtual FloatLiteralExpr    *asFloatLiteralExpr();
    virtual TupleExpr           *asTupleExpr();
    virtual MemExpr             *asMemExpr();
    virtual NewExpr             *asNewExpr();
    virtual DeleteExpr          *asDeleteExpr();
    virtual RetainExpr          *asRetainExpr();
    virtual ReleaseExpr         *asReleaseExpr();
    virtual ThisExpr            *asThisExpr();
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
    virtual bool isIntLiteralExpr();
    virtual bool isFloatLiteralExpr();
    virtual bool isTupleExpr();
    virtual bool isMemExpr();
    virtual bool isNewExpr();
    virtual bool isDeleteExpr();
    virtual bool isRetainExpr();
    virtual bool isReleaseExpr();
    virtual bool isThisExpr();
    virtual bool isIdExpr();
    virtual bool isPackExpr();
    virtual bool isMemberExpr();
    virtual bool isCallExpr();
    virtual bool isIndexExpr();
    virtual bool isCastExpr();

    virtual Type *getType() {return NULL;} //= 0; //TODO pure virtual
    virtual void serialize(StringFormatter &sfmt);
};

class BinaryExpr : public Expr {
    Expr *lhs;
    Expr *rhs;
    virtual String serializeName() = 0;

    public:
    BinaryExpr(Expr *_lhs, Expr *rhs);
    virtual ~BinaryExpr();
    virtual BinaryExpr *asBinaryExpr();
    virtual void serialize(StringFormatter &sfmt);
};

class ModulusExpr : public BinaryExpr {
    virtual String serializeName();
    public:
    ModulusExpr(Expr *_lhs, Expr *_rhs);
};

class PowerExpr : public BinaryExpr {
    virtual String serializeName();
    public:
    PowerExpr(Expr *_lhs, Expr *_rhs);
};

class DivExpr : public BinaryExpr {
    virtual String serializeName();
    public:
    DivExpr(Expr *_lhs, Expr *_rhs);
};

class MulExpr : public BinaryExpr {
    virtual String serializeName();
    public:
    MulExpr(Expr *_lhs, Expr *_rhs);
};

class LShiftExpr : public BinaryExpr {
    virtual String serializeName();
    public:
    LShiftExpr(Expr *_lhs, Expr *_rhs);
};

class RShiftExpr : public BinaryExpr {
    virtual String serializeName();
    public:
    RShiftExpr(Expr *_lhs, Expr *_rhs);
};

class AddExpr : public BinaryExpr {
    virtual String serializeName();
    public:
    AddExpr(Expr *_lhs, Expr *_rhs);
};

class SubExpr : public BinaryExpr {
    virtual String serializeName();
    public:
    SubExpr(Expr *_lhs, Expr *_rhs);
};

class BitXorExpr : public BinaryExpr {
    virtual String serializeName();
    public:
    BitXorExpr(Expr *_lhs, Expr *_rhs);
};

class BitAndExpr : public BinaryExpr {
    virtual String serializeName();
    public:
    BitAndExpr(Expr *_lhs, Expr *_rhs);
};

class BitOrExpr : public BinaryExpr {
    virtual String serializeName();
    public:
    BitOrExpr(Expr *_lhs, Expr *_rhs);
};

class AndExpr : public BinaryExpr {
    virtual String serializeName();
    public:
    AndExpr(Expr *_lhs, Expr *_rhs);
};

class OrExpr : public BinaryExpr {
    virtual String serializeName();
    public:
    OrExpr(Expr *_lhs, Expr *_rhs);
};

class RelationExpr : public BinaryExpr {
    public:
    RelationExpr(Expr *_lhs, Expr *_rhs);
};

class GreaterThanExpr : public RelationExpr {
    virtual String serializeName();
    public:
    GreaterThanExpr(Expr *_lhs, Expr *_rhs);
};

class GreaterOrEqualExpr : public RelationExpr {
    virtual String serializeName();
    public:
    GreaterOrEqualExpr(Expr *_lhs, Expr *_rhs);
};

class LessThanExpr : public RelationExpr {
    virtual String serializeName();
    public:
    LessThanExpr(Expr *_lhs, Expr *_rhs);
};

class LessOrEqualExpr : public RelationExpr {
    virtual String serializeName();
    public:
    LessOrEqualExpr(Expr *_lhs, Expr *_rhs);
};

class NotEqualExpr : public RelationExpr {
    virtual String serializeName();
    public:
    NotEqualExpr(Expr *_lhs, Expr *_rhs);
};

class EqualExpr : public RelationExpr {
    virtual String serializeName();
    public:
    EqualExpr(Expr *_lhs, Expr *_rhs);
};

class UnaryExpr : public Expr {
    Expr *operand;

    virtual String serializeName() = 0;
    public:
    UnaryExpr(Expr *_operand);
    ~UnaryExpr();
    virtual UnaryExpr *asUnaryExpr();
    virtual Type *getType(); //TODO
    virtual void serialize(StringFormatter &sfmt);
};

class PreIncExpr : public UnaryExpr {
    virtual String serializeName();

    public:
    PreIncExpr(Expr *_operand);
};

class PreDecExpr : public UnaryExpr {
    virtual String serializeName();

    public:
    PreDecExpr(Expr *_operand);
};

class NegateExpr : public UnaryExpr {
    virtual String serializeName();

    public:
    NegateExpr(Expr *_operand);
};

class NotExpr : public UnaryExpr {
    virtual String serializeName();

    public:
    NotExpr(Expr *_operand);
};

class BitNotExpr : public UnaryExpr {
    virtual String serializeName();

    public:
    BitNotExpr(Expr *_operand);
};

class DerefExpr : public UnaryExpr {
    virtual String serializeName();

    public:
    DerefExpr(Expr *_operand);
};

class RefExpr : public UnaryExpr {
    virtual String serializeName();

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
    StringLiteralExpr(String _value);
    virtual StringLiteralExpr *asStringLiteralExpr();
    virtual void serialize(StringFormatter &sfmt);
};

class BoolLiteralExpr : public LiteralExpr {
    bool value;

    public:
    BoolLiteralExpr(bool _value);
    virtual BoolLiteralExpr *asBoolLiteralExpr();
    virtual void serialize(StringFormatter &sfmt);
};

class NullLiteralExpr : public LiteralExpr {
    public:
    virtual NullLiteralExpr *asNullLiteralExpr();
    virtual void serialize(StringFormatter &sfmt);
};

class NumericLiteralExpr : public LiteralExpr {
    public:
    virtual NumericLiteralExpr *asNumericLiteralExpr();
};

class IntLiteralExpr : public NumericLiteralExpr {
    public:
    uint64_t value;
    public:
    IntLiteralExpr(uint64_t _value);
    virtual IntLiteralExpr *asIntLiteralExpr();
    virtual void serialize(StringFormatter &sfmt);
};

class FloatLiteralExpr : public NumericLiteralExpr {
    double value;
    public:
    FloatLiteralExpr(double _value);
    virtual FloatLiteralExpr *asFloatLiteralExpr();
    virtual void serialize(StringFormatter &sfmt);
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
    IdExpr(String _name);
    virtual IdExpr *asIdExpr();
    virtual void serialize(StringFormatter &sfmt);
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
