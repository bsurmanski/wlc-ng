#include "expr.hpp"

#include "exception/exception.hpp"

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

IntLiteralExpr *Expr::asIntLiteralExpr() {
    return NULL;
}

FloatLiteralExpr *Expr::asFloatLiteralExpr() {
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

ThisExpr *Expr::asThisExpr() {
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

bool Expr::isIntLiteralExpr() {
    return (bool) asIntLiteralExpr();
}

bool Expr::isFloatLiteralExpr() {
    return (bool) asFloatLiteralExpr();
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

bool Expr::isThisExpr() {
    return (bool) asThisExpr();
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

void Expr::serialize(StringFormatter &sfmt) {
    sfmt.write("(expr)");
}

/*
 * BinaryExpr
 */

BinaryExpr::BinaryExpr(Expr *_lhs, Expr *_rhs) : lhs(_lhs), rhs(_rhs) {
}

BinaryExpr::~BinaryExpr() {
    delete lhs;
    delete rhs;
}

BinaryExpr *BinaryExpr::asBinaryExpr() {
    return this;
}

void BinaryExpr::serialize(StringFormatter &sfmt) {
    sfmt.write("(");
    sfmt.write(serializeName());
    sfmt.write(" ");
    lhs->serialize(sfmt);
    sfmt.write(" ");
    rhs->serialize(sfmt);
    sfmt.write(")");
}

ModulusExpr::ModulusExpr(Expr *_lhs, Expr *_rhs) : BinaryExpr(_lhs, _rhs) {
}

String ModulusExpr::serializeName() {
    return "mod";
}

PowerExpr::PowerExpr(Expr *_lhs, Expr *_rhs) : BinaryExpr(_lhs, _rhs) {
}

String PowerExpr::serializeName() {
    return "pow";
}

DivExpr::DivExpr(Expr *_lhs, Expr *_rhs) : BinaryExpr(_lhs, _rhs) {
}

String DivExpr::serializeName() {
    return "div";
}

MulExpr::MulExpr(Expr *_lhs, Expr *_rhs) : BinaryExpr(_lhs, _rhs) {
}

String MulExpr::serializeName() {
    return "mul";
}

LShiftExpr::LShiftExpr(Expr *_lhs, Expr *_rhs) : BinaryExpr(_lhs, _rhs) {
}

String LShiftExpr::serializeName() {
    return "lshift";
}

RShiftExpr::RShiftExpr(Expr *_lhs, Expr *_rhs) : BinaryExpr(_lhs, _rhs) {
}

String RShiftExpr::serializeName() {
    return "rshift";
}

AddExpr::AddExpr(Expr *_lhs, Expr *_rhs) : BinaryExpr(_lhs, _rhs) {
}

String AddExpr::serializeName() {
    return "add";
}

SubExpr::SubExpr(Expr *_lhs, Expr *_rhs) : BinaryExpr(_lhs, _rhs) {
}

String SubExpr::serializeName() {
    return "sub";
}

BitXorExpr::BitXorExpr(Expr *_lhs, Expr *_rhs) : BinaryExpr(_lhs, _rhs) {
}

String BitXorExpr::serializeName() {
    return "bitxor";
}

BitAndExpr::BitAndExpr(Expr *_lhs, Expr *_rhs) : BinaryExpr(_lhs, _rhs) {
}

String BitAndExpr::serializeName() {
    return "bitand";
}

BitOrExpr::BitOrExpr(Expr *_lhs, Expr *_rhs) : BinaryExpr(_lhs, _rhs) {
}

String BitOrExpr::serializeName() {
    return "bitor";
}

AndExpr::AndExpr(Expr *_lhs, Expr *_rhs) : BinaryExpr(_lhs, _rhs) {
}

String AndExpr::serializeName() {
    return "and";
}

OrExpr::OrExpr(Expr *_lhs, Expr *_rhs) : BinaryExpr(_lhs, _rhs) {
}

String OrExpr::serializeName() {
    return "or";
}

RelationExpr::RelationExpr(Expr *_lhs, Expr *_rhs) : BinaryExpr(_lhs, _rhs) {
}

GreaterThanExpr::GreaterThanExpr(Expr *_lhs, Expr *_rhs) : RelationExpr(_lhs, _rhs) {
}

String GreaterThanExpr::serializeName() {
    return "gt";
}

GreaterOrEqualExpr::GreaterOrEqualExpr(Expr *_lhs, Expr *_rhs) : RelationExpr(_lhs, _rhs) {
}

String GreaterOrEqualExpr::serializeName() {
    return "ge";
}

LessThanExpr::LessThanExpr(Expr *_lhs, Expr *_rhs) : RelationExpr(_lhs, _rhs) {
}

String LessThanExpr::serializeName() {
    return "lt";
}

LessOrEqualExpr::LessOrEqualExpr(Expr *_lhs, Expr *_rhs) : RelationExpr(_lhs, _rhs) {
}

String LessOrEqualExpr::serializeName() {
    return "le";
}

NotEqualExpr::NotEqualExpr(Expr *_lhs, Expr *_rhs) : RelationExpr(_lhs, _rhs) {
}

String NotEqualExpr::serializeName() {
    return "ne";
}

EqualExpr::EqualExpr(Expr *_lhs, Expr *_rhs) : RelationExpr(_lhs, _rhs) {
}

String EqualExpr::serializeName() {
    return "eq";
}

/*
 * UnaryExpr
 */

UnaryExpr::UnaryExpr(Expr *_operand) : operand(_operand) {
}

UnaryExpr::~UnaryExpr() {
    delete operand;
}

UnaryExpr *UnaryExpr::asUnaryExpr() {
    return this;
}

Type *UnaryExpr::getType() {
    return NULL; //TODO
}

void UnaryExpr::serialize(StringFormatter &sfmt) {
    sfmt.write("(");
    sfmt.write(serializeName());
    sfmt.write(" ");
    operand->serialize(sfmt);
    sfmt.write(")");
}

PreIncExpr::PreIncExpr(Expr *_operand) : UnaryExpr(_operand) {
}

String PreIncExpr::serializeName() {
    return "preinc";
}

PreDecExpr::PreDecExpr(Expr *_operand) : UnaryExpr(_operand) {
}

String PreDecExpr::serializeName() {
    return "predec";
}

PostIncExpr::PostIncExpr(Expr *_operand) : UnaryExpr(_operand) {
}

String PostIncExpr::serializeName() {
    return "postinc";
}

PostDecExpr::PostDecExpr(Expr *_operand) : UnaryExpr(_operand) {
}

String PostDecExpr::serializeName() {
    return "postdec";
}

NegateExpr::NegateExpr(Expr *_operand) : UnaryExpr(_operand) {
}

String NegateExpr::serializeName() {
    return "neg";
}

NotExpr::NotExpr(Expr *_operand) : UnaryExpr(_operand) {
}

String NotExpr::serializeName() {
    return "not";
}

BitNotExpr::BitNotExpr(Expr *_operand) : UnaryExpr(_operand) {
}

String BitNotExpr::serializeName() {
    return "bitnot";
}

DerefExpr::DerefExpr(Expr *_operand) : UnaryExpr(_operand) {
}

String DerefExpr::serializeName() {
    return "deref";
}

RefExpr::RefExpr(Expr *_operand) : UnaryExpr(_operand) {
}

String RefExpr::serializeName() {
    return "ref";
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

void BoolLiteralExpr::serialize(StringFormatter &sfmt) {
    if(value) {
        sfmt.write("true");
    } else {
        sfmt.write("false");
    }
}

NullLiteralExpr *NullLiteralExpr::asNullLiteralExpr() {
    return this;
}

void NullLiteralExpr::serialize(StringFormatter &sfmt) {
    sfmt.write("null");
}

StringLiteralExpr::StringLiteralExpr(String _value) : value(_value) {
}

StringLiteralExpr *StringLiteralExpr::asStringLiteralExpr() {
    return this;
}

void StringLiteralExpr::serialize(StringFormatter &sfmt) {
    //TODO: escape string
    sfmt.write("\"");
    sfmt.write(value.escapedString());
    sfmt.write("\"");
}

NumericLiteralExpr *NumericLiteralExpr::asNumericLiteralExpr() {
    return this;
}

IntLiteralExpr::IntLiteralExpr(uint64_t _value) : value(_value) {
}

IntLiteralExpr *IntLiteralExpr::asIntLiteralExpr() {
    return this;
}

void IntLiteralExpr::serialize(StringFormatter &sfmt) {
    sfmt.write(String::fromUInt(value));
}

FloatLiteralExpr::FloatLiteralExpr(double _value) : value(_value) {
}

FloatLiteralExpr *FloatLiteralExpr::asFloatLiteralExpr() {
    return this;
}

void FloatLiteralExpr::serialize(StringFormatter &sfmt) {
    sfmt.write(String::fromFloat(value));
}

MemExpr *MemExpr::asMemExpr() {
    return this;
}

NewExpr::NewExpr(Type *_type) : type(_type) {
}

NewExpr *NewExpr::asNewExpr() {
    return this;
}

void NewExpr::serialize(StringFormatter &sfmt) {
    sfmt.write("(new ");
    type->serialize(sfmt);
    sfmt.write(")");
}

DeleteExpr::DeleteExpr(Expr *_operand) : operand(_operand) {
}

DeleteExpr *DeleteExpr::asDeleteExpr() {
    return this;
}

void DeleteExpr::serialize(StringFormatter &sfmt) {
    sfmt.write("(delete ");
    operand->serialize(sfmt);
    sfmt.write(")");
}

RetainExpr::RetainExpr(Expr *_operand) : operand(_operand) {
}

RetainExpr *RetainExpr::asRetainExpr() {
    return this;
}

void RetainExpr::serialize(StringFormatter &sfmt) {
    sfmt.write("(retain ");
    operand->serialize(sfmt);
    sfmt.write(")");
}

ReleaseExpr::ReleaseExpr(Expr *_operand) : operand(_operand) {
}

ReleaseExpr *ReleaseExpr::asReleaseExpr() {
    return this;
}

void ReleaseExpr::serialize(StringFormatter &sfmt) {
    sfmt.write("(release ");
    operand->serialize(sfmt);
    sfmt.write(")");
}

IdExpr::IdExpr(String _name) : name(_name) {
}

IdExpr *IdExpr::asIdExpr() {
    return this;
}

void IdExpr::serialize(StringFormatter &sfmt) {
    sfmt.write("(id ");
    sfmt.write(name);
    sfmt.write(")");
}

PackExpr::PackExpr(const String &_filename) : filename(_filename) {
}

PackExpr *PackExpr::asPackExpr() {
    return this;
}

void PackExpr::serialize(StringFormatter &sfmt) {
    sfmt.write("(pack \"");
    sfmt.write(filename.escapedString());
    sfmt.write("\")");
}

MemberExpr *MemberExpr::asMemberExpr() {
    return this;
}

MemberExpr::MemberExpr(Expr *_base, const String &_memberId) : base(_base), memberId(_memberId) {
}

void MemberExpr::serialize(StringFormatter &sfmt) {
    sfmt.write("(member ");
    base->serialize(sfmt);
    sfmt.write(" (id ");
    sfmt.write(memberId);
    sfmt.write("))");
}

CallExpr::CallExpr(Expr *_func, DynArray<Expr*> _args) : func(_func), args(_args) {
}

CallExpr *CallExpr::asCallExpr() {
    return this;
}

void CallExpr::serialize(StringFormatter &sfmt) {
    sfmt.write("(call ");
    func->serialize(sfmt);
    for(int i = 0; i < args.size(); i++) {
        sfmt.write(" ");
        args[i]->serialize(sfmt);
    }
    sfmt.write(")");
}

IndexExpr::IndexExpr(Expr *_base, Expr *_index) : base(_base), index(_index) {
}

IndexExpr *IndexExpr::asIndexExpr() {
    return this;
}

void IndexExpr::serialize(StringFormatter &sfmt) {
    sfmt.write("(index ");
    base->serialize(sfmt);
    sfmt.write(" ");
    index->serialize(sfmt);
    sfmt.write(")");
}
