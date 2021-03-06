#include <gtest/gtest.h>
#include <stdio.h>

#include "ast/ast.hpp"
#include "common/string.hpp"
#include "io/environment.hpp"
#include "io/stringInput.hpp"
#include "lex/lexer.hpp"
#include "parse/parser.hpp"
#include "program.hpp"

#define TRY(X) { try { (X); } catch(std::exception &e) { printf("%s\n", e.what()); ADD_FAILURE(); }}

class TestParser : public testing::Test {
    Parser *parser;
    public:
    Program *newDummyProgram() {
        Environment *e = new Environment;
        Program *p = new Program(e);
        return p;
    }

    Parser *newStringParser(String str) {
        Program *prog = newDummyProgram();
        Lexer *lex = new Lexer(new StringInput(str));
        return new Parser(prog, lex);
    }

    void TestStmt(String serialized_exp, String stmt_str) {
        Parser *parser;
        Stmt *stmt;
        TRY({
                parser = newStringParser(stmt_str);
                stmt = parser->parseStmt();
                EXPECT_EQ(serialized_exp, stmt->serialized());
                delete stmt;
            });
        delete parser;
    }

    void TestExpr(String serialized_exp, String expr_str) {
        Parser *parser;
        Expr *expr;
        TRY({
                parser = newStringParser(expr_str);
                expr = parser->parseExpr();
                EXPECT_EQ(serialized_exp, expr->serialized());
                delete expr;
            });
        delete parser;
    }

    void TestDecl(String serialized_exp, String decl_str) {
        Parser *parser;
        Decl *decl;
        TRY({
                parser = newStringParser(decl_str);
                decl = parser->parseDecl();
                EXPECT_EQ(serialized_exp, decl->serialized());
                delete decl;
            });
        delete parser;
    }

    void TestType(String serialized_exp, String type_str) {
        Parser *parser;
        Type *type;
        TRY({
                parser = newStringParser(type_str);
                type = parser->parseType();
                EXPECT_EQ(serialized_exp, type->serialized());
            });
        delete parser;
    }

    virtual void OnTestStart(const testing::TestInfo &test_info) {
    }

    virtual void OnTestEnd(const testing::TestInfo &test_info) {
    }
};

static Program *createDummyProgram() {
    Environment *e = new Environment;
    Program *p = new Program(e);
    return p;
}

static Parser *createStringParser(String str) {
    Program *prog = createDummyProgram();
    Lexer *lex = new Lexer(new StringInput(str));
    return new Parser(prog, lex);
}

TEST_F(TestParser, CompoundStmt) {
    TestStmt("(stmts\n  (var i int32 0)\n  (if (lt (id i) 0)\n    (set (id i) 5)))", "{\nint i = 0\nif(i < 0) i = 5\n}");
    TestStmt("(stmts\n  (var i int32 0)\n  (if (lt (id i) 0)\n    (set (id i) 5)))", "{int i = 0;if(i < 0) i = 5}");
}

TEST_F(TestParser, ReturnStmt) {
    TestStmt("(return 0)", "return 0");
    TestStmt("(return)", "return \n 0");
}

TEST_F(TestParser, PrimativeType) {
    Parser *parser = NULL;
    PrimativeType *type = NULL;

#define TEST_STR(EXP, STR) \
    parser = createStringParser(STR);\
    type = parser->parsePrimativeType();\
    EXPECT_NE((Type*) NULL, type);\
    if(type) {\
        EXPECT_EQ(EXP, type->getKind());\
        EXPECT_EQ(true, parser->getLexer()->eof());\
    }\
    delete parser;

    TRY({
        TEST_STR(PrimativeType::BOOL, "bool");
        TEST_STR(PrimativeType::VOID, "void");

        TEST_STR(PrimativeType::INT8, "char");
        TEST_STR(PrimativeType::INT16, "short");
        TEST_STR(PrimativeType::INT32, "int");
        TEST_STR(PrimativeType::INT64, "long");

        TEST_STR(PrimativeType::INT8, "int8");
        TEST_STR(PrimativeType::INT16, "int16");
        TEST_STR(PrimativeType::INT32, "int32");
        TEST_STR(PrimativeType::INT64, "int64");

        TEST_STR(PrimativeType::UINT8, "uchar");
        TEST_STR(PrimativeType::UINT16, "ushort");
        TEST_STR(PrimativeType::UINT32, "uint");
        TEST_STR(PrimativeType::UINT64, "ulong");

        TEST_STR(PrimativeType::UINT8, "uint8");
        TEST_STR(PrimativeType::UINT16, "uint16");
        TEST_STR(PrimativeType::UINT32, "uint32");
        TEST_STR(PrimativeType::UINT64, "uint64");

        TEST_STR(PrimativeType::FLOAT32, "float");
        TEST_STR(PrimativeType::FLOAT64, "double");

        TEST_STR(PrimativeType::FLOAT32, "float32");
        TEST_STR(PrimativeType::FLOAT64, "float64");
    });
#undef TEST_STR

    TestType("bool", "bool");
    TestType("void", "void");
    TestType("int8", "int8");
    TestType("int16", "int16");
    TestType("int32", "int32");
    TestType("int64", "int64");

    TestType("uint8", "uint8");
    TestType("uint16", "uint16");
    TestType("uint32", "uint32");
    TestType("uint64", "uint64");
    TestType("float32", "float32");
    TestType("float64", "float64");

}

TEST_F(TestParser, UnaryExpr) {
    TestExpr("(preinc 1)", "++1");
    TestExpr("(predec 1)", "--1");
    TestExpr("(not null)", "!null");
    TestExpr("(deref null)", "^null");
    TestExpr("(ref null)", "&null");
}

TEST_F(TestParser, BinaryExpr) {
    TestExpr("(add 1 1)", "1 + 1");
    TestExpr("(add (add 1 2) 3)", "1 + 2 + 3");
    TestExpr("(add 1 (mul 2 3))", "1 + 2 * 3");
    TestExpr("(add 1 (mul (mul 2 3) 4))", "1 + 2 * 3 * 4");
    TestExpr("(sub (add 1 (div 2 3)) 4)", "1 + 2 / 3 - 4");
}

TEST_F(TestParser, PostfixExpr) {
    TestExpr("(postinc 1)", "1++");
    TestExpr("(postdec 1)", "1--");
}

TEST_F(TestParser, IdExpr) {
    TestExpr("(id myid)", "myid");
    TestExpr("(id myid)", "myid ;");
}

TEST_F(TestParser, RetainExpr) {
    TestExpr("(retain (id a))", "retain a");
}

TEST_F(TestParser, ReleaseExpr) {
    TestExpr("(release (id a))", "release a");
}

TEST_F(TestParser, NewExpr) {
    TestExpr("(new (type mytype))", "new mytype");
}

TEST_F(TestParser, DeleteExpr) {
    TestExpr("(delete (id a))", "delete a");
}

TEST_F(TestParser, CallExpr) {
    TestExpr("(call (id myfunc))", "myfunc()");
    TestExpr("(call (id myfunc) 1 2 3 (id a) (id b))", "myfunc(1, 2, 3, a, b)");
}

TEST_F(TestParser, MemberExpr) {
    TestExpr("(member (id a) (id b))", "a.b");
    TestExpr("(member (member (id a) (id b)) (id c))", "a.b.c");
}

TEST_F(TestParser, IndexExpr) {
    TestExpr("(index (id a) 0)", "a[0]");
    TestExpr("(index (id abc) (add 1 2))", "abc[1 + 2]");
}

TEST_F(TestParser, PackExpr) {
    TestExpr("(pack \"afile.txt\")", "pack \"afile.txt\"");
}

TEST_F(TestParser, CastExpr) {
    TestExpr("(cast (id myexpr) int)", "int: myexpr");
}

TEST_F(TestParser, Literals) {
    TestExpr("123", "123");
    TestExpr("true", "true");
    TestExpr("false", "false");
    TestExpr("null", "null");
}

TEST_F(TestParser, ComplexExpr) {
    TestExpr("(sub 3 (mul 1 2))", "3 - (1 * 2)");
    TestExpr("(sub (mul 1 2) 3)", "(1 * 2) - 3");
    TestExpr("(mul (add (id a) (id b)) (div 2 3))", "(a + b) * (2 / 3)");
    TestExpr("(mul (neg (id a)) (neg (id b)))", "-a * -b");
}


TEST_F(TestParser, VarDecl) {
    TestDecl("(var hello int8)", "int8 hello");
    TestDecl("(var goodbye (ptr float32))", "float^ goodbye");
    TestDecl("(var myvar int32)", "int myvar");
}

TEST_F(TestParser, FuncDecl) {
    TestDecl("(func hello void (vars (var a int8))\n  (break))", "void hello(int8 a) break");
    TestDecl("(func hello void (vars (var a int8))\n  (stmts\n    (call (id printf) \"helloworld\")))", "void hello(int8 a) {\nprintf(\"helloworld\")\n}");
}

TEST_F(TestParser, TypeDecl) {
    TestDecl("(struct mystruct (decls\n  (var i int32)\n  (var j int32)\n  (var k (type mytype))))", "struct mystruct{\nint i\nint j\nmytype k\n}");
    TestDecl("(struct mystruct (decls))", "struct mystruct{}");
    TestDecl("(class myclass (id base) (decls))", "class myclass : base {}");
}
