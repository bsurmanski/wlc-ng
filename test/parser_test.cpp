#include <gtest/gtest.h>
#include <stdio.h>

#include "ast/ast.hpp"
#include "common/string.hpp"
#include "io/environment.hpp"
#include "io/stringInput.hpp"
#include "lex/lexer.hpp"
#include "parse/parser.hpp"
#include "program.hpp"

#define TRY(X) { try { (X); } catch(std::exception *e) { printf("%s\n", e->what()); FAIL(); }}

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
                EXPECT_EQ(serialized_exp, stmt->serialize());
                delete stmt;
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

TEST(Parser, CompoundStmt) {
    String in("{\nint i = 0\n if(i < 0) i = 5\n}");
    Parser *parser = createStringParser(in);
    Stmt *stmt;
    TRY((stmt = parser->parseStmt()));
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
    if(type) {\
        EXPECT_EQ(EXP, type->getKind());\
        EXPECT_EQ(true, parser->getLexer()->eof());\
    } else {\
        EXPECT_NE((Type*) NULL, type);\
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
}

TEST(Parser, UnaryExpr) {
    UnaryExpr *uexp;
    Parser *parser;
#define TEST_STR(EXP, STR)\
    parser = createStringParser(STR);\
    uexp = parser->parseUnaryExpr();\
    EXPECT_EQ(String(EXP), uexp->serialize());\
    delete uexp;\
    delete parser;

    TRY({
        TEST_STR("(preinc true)", "++true");
        TEST_STR("(predec false)", "--false");
        TEST_STR("(not null)", "!null");
        TEST_STR("(deref null)", "^null");
        TEST_STR("(ref null)", "&null");
    });

#undef TEST_STR
}

TEST(Parser, IdExpr) {
    Expr *expr;
    Parser *parser;

#define TEST_STR(EXP, STR)\
    parser = createStringParser(STR);\
    expr = parser->parseExpr();\
    EXPECT_EQ(String(EXP), expr->serialize());\
    delete expr;\
    delete parser;

    EXPECT_EQ(String("(id someid)"), IdExpr("someid").serialize());
    TEST_STR("(id myid)", "myid");
    TEST_STR("(id myid)", "myid myid");

#undef TEST_STR
}

TEST(Parser, Literals) {
    Parser *parser;
    Expr *expr;
#define TEST_STR(EXP, STR)\
    parser = createStringParser(String(STR));\
    expr = parser->parseExpr();\
    EXPECT_EQ(String(EXP), expr->serialize());\
    delete expr;\
    delete parser;

    TEST_STR("123", "123");
    TEST_STR("true", "true");
    TEST_STR("false", "false");
    TEST_STR("null", "null");

#undef TEST_STR
}

