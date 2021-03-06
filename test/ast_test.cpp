#include <gtest/gtest.h>

#include "ast/ast.hpp"

TEST(AST, SerializePrimativeType) {
    EXPECT_EQ(String("bool"), PrimativeType::getBool()->serialized());
    EXPECT_EQ(String("void"), PrimativeType::getVoid()->serialized());
    EXPECT_EQ(String("int8"), PrimativeType::getInt8()->serialized());
    EXPECT_EQ(String("int16"), PrimativeType::getInt16()->serialized());
    EXPECT_EQ(String("int32"), PrimativeType::getInt32()->serialized());
    EXPECT_EQ(String("int64"), PrimativeType::getInt64()->serialized());
    EXPECT_EQ(String("uint8"), PrimativeType::getUInt8()->serialized());
    EXPECT_EQ(String("uint16"), PrimativeType::getUInt16()->serialized());
    EXPECT_EQ(String("uint32"), PrimativeType::getUInt32()->serialized());
    EXPECT_EQ(String("uint64"), PrimativeType::getUInt64()->serialized());
    EXPECT_EQ(String("float32"), PrimativeType::getFloat32()->serialized());
    EXPECT_EQ(String("float64"), PrimativeType::getFloat64()->serialized());

    EXPECT_EQ(String("(ptr bool)"), PrimativeType::getBool()->getPointer()->serialized());
}

TEST(AST, SerializeStmt) {
    EXPECT_EQ(String("(continue)"), ContinueStmt().serialized());
    EXPECT_EQ(String("(break)"), BreakStmt().serialized());
    EXPECT_EQ(String("(goto somelabel)"), GotoStmt(String("somelabel")).serialized());
    EXPECT_EQ(String("(label somelabel)"), LabelStmt(String("somelabel")).serialized());
    EXPECT_EQ(String("(if true\n  (continue)\n  (break))"), IfStmt(new BoolLiteralExpr(true), new ContinueStmt, new BreakStmt).serialized());

    DynArray<Stmt*> stmts;
    stmts.append(new BreakStmt);
    stmts.append(new ContinueStmt);
    EXPECT_EQ(String("(stmts\n  (break)\n  (continue))"), CompoundStmt(stmts).serialized());

    EXPECT_EQ(String("(set (id a) 5)"), AssignStmt(new IdExpr("a"), new IntLiteralExpr(5)).serialized());
    //TODO: caseStmt
    //TODO: loopStmt (and subclasses)
}

TEST(AST, SerializeLiteral) {
    EXPECT_EQ(String("1.000000"), FloatLiteralExpr(1.0).serialized());
    EXPECT_EQ(String("123"), IntLiteralExpr(123).serialized());
    EXPECT_EQ(String("null"), NullLiteralExpr().serialized());
    EXPECT_EQ(String("true"), BoolLiteralExpr(true).serialized());
    EXPECT_EQ(String("false"), BoolLiteralExpr(false).serialized());
    EXPECT_EQ(String("\"somestring\""), StringLiteralExpr(String("somestring")).serialized());
}

TEST(AST, SerializeUnaryExpr) {
    EXPECT_EQ(String("(preinc null)"), PreIncExpr(new NullLiteralExpr).serialized());
    EXPECT_EQ(String("(predec null)"), PreDecExpr(new NullLiteralExpr).serialized());
    EXPECT_EQ(String("(neg 987)"), NegateExpr(new IntLiteralExpr(987)).serialized());
    EXPECT_EQ(String("(not null)"), NotExpr(new NullLiteralExpr).serialized());
    EXPECT_EQ(String("(deref null)"), DerefExpr(new NullLiteralExpr).serialized());
    EXPECT_EQ(String("(ref null)"), RefExpr(new NullLiteralExpr).serialized());
}

TEST(AST, SerializeBinaryExpr) {
    EXPECT_EQ(String("(mod 1 2)"), ModulusExpr(new IntLiteralExpr(1), new IntLiteralExpr(2)).serialized());
    EXPECT_EQ(String("(pow 2 10)"), PowerExpr(new IntLiteralExpr(2), new IntLiteralExpr(10)).serialized());
    EXPECT_EQ(String("(div 10 2)"), DivExpr(new IntLiteralExpr(10), new IntLiteralExpr(2)).serialized());
    EXPECT_EQ(String("(mul 2 2)"), MulExpr(new IntLiteralExpr(2), new IntLiteralExpr(2)).serialized());
    EXPECT_EQ(String("(lshift 8 2)"), LShiftExpr(new IntLiteralExpr(8), new IntLiteralExpr(2)).serialized());
    EXPECT_EQ(String("(rshift 8 2)"), RShiftExpr(new IntLiteralExpr(8), new IntLiteralExpr(2)).serialized());
    EXPECT_EQ(String("(add 8 2)"), AddExpr(new IntLiteralExpr(8), new IntLiteralExpr(2)).serialized());
    EXPECT_EQ(String("(sub 8 2)"), SubExpr(new IntLiteralExpr(8), new IntLiteralExpr(2)).serialized());
    EXPECT_EQ(String("(bitxor 1 3)"), BitXorExpr(new IntLiteralExpr(1), new IntLiteralExpr(3)).serialized());
    EXPECT_EQ(String("(bitand 1 3)"), BitAndExpr(new IntLiteralExpr(1), new IntLiteralExpr(3)).serialized());
    EXPECT_EQ(String("(bitor 1 3)"), BitOrExpr(new IntLiteralExpr(1), new IntLiteralExpr(3)).serialized());
    EXPECT_EQ(String("(or true true)"), OrExpr(new BoolLiteralExpr(true), new BoolLiteralExpr(true)).serialized());
    EXPECT_EQ(String("(and true true)"), AndExpr(new BoolLiteralExpr(true), new BoolLiteralExpr(true)).serialized());
}
