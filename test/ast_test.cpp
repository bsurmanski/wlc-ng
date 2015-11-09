#include <gtest/gtest.h>

#include "ast/ast.hpp"

TEST(AST, SerializeLiteral) {
    EXPECT_EQ(String("1.0"), FloatLiteralExpr(1.0).serialize());
    EXPECT_EQ(String("123"), IntLiteralExpr(123).serialize());
    EXPECT_EQ(String("null"), NullLiteralExpr().serialize());
    EXPECT_EQ(String("true"), BoolLiteralExpr(true).serialize());
    EXPECT_EQ(String("false"), BoolLiteralExpr(false).serialize());
}

TEST(AST, SerializeUnaryExpr) {
    EXPECT_EQ(String("(preinc null)"), PreIncExpr(new NullLiteralExpr).serialize());
    EXPECT_EQ(String("(predec null)"), PreDecExpr(new NullLiteralExpr).serialize());
    EXPECT_EQ(String("(neg 987)"), NegateExpr(new IntLiteralExpr(987)).serialize());
    EXPECT_EQ(String("(not null)"), NotExpr(new NullLiteralExpr).serialize());
    EXPECT_EQ(String("(deref null)"), DerefExpr(new NullLiteralExpr).serialize());
    EXPECT_EQ(String("(ref null)"), RefExpr(new NullLiteralExpr).serialize());
}

TEST(AST, SerializeBinaryExpr) {
    EXPECT_EQ(String("(mod 1 2)"), ModulusExpr(new IntLiteralExpr(1), new IntLiteralExpr(2)).serialize());
    EXPECT_EQ(String("(pow 2 10)"), PowerExpr(new IntLiteralExpr(2), new IntLiteralExpr(10)).serialize());
    EXPECT_EQ(String("(div 10 2)"), DivExpr(new IntLiteralExpr(10), new IntLiteralExpr(2)).serialize());
    EXPECT_EQ(String("(mul 2 2)"), MulExpr(new IntLiteralExpr(2), new IntLiteralExpr(2)).serialize());
    EXPECT_EQ(String("(lshift 8 2)"), LShiftExpr(new IntLiteralExpr(8), new IntLiteralExpr(2)).serialize());
    EXPECT_EQ(String("(rshift 8 2)"), RShiftExpr(new IntLiteralExpr(8), new IntLiteralExpr(2)).serialize());
    EXPECT_EQ(String("(add 8 2)"), AddExpr(new IntLiteralExpr(8), new IntLiteralExpr(2)).serialize());
    EXPECT_EQ(String("(sub 8 2)"), SubExpr(new IntLiteralExpr(8), new IntLiteralExpr(2)).serialize());
    EXPECT_EQ(String("(bitxor 1 3)"), BitXorExpr(new IntLiteralExpr(1), new IntLiteralExpr(3)).serialize());
    EXPECT_EQ(String("(bitand 1 3)"), BitAndExpr(new IntLiteralExpr(1), new IntLiteralExpr(3)).serialize());
    EXPECT_EQ(String("(bitor 1 3)"), BitOrExpr(new IntLiteralExpr(1), new IntLiteralExpr(3)).serialize());
    EXPECT_EQ(String("(or true true)"), OrExpr(new BoolLiteralExpr(true), new BoolLiteralExpr(true)).serialize());
    EXPECT_EQ(String("(and true true)"), AndExpr(new BoolLiteralExpr(true), new BoolLiteralExpr(true)).serialize());
}
