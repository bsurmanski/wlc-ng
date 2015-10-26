#include <gtest/gtest.h>
#include <stdio.h>

#include "program.hpp"
#include "io/environment.hpp"
#include "io/stringInput.hpp"
#include "common/string.hpp"
#include "parse/parser.hpp"

#define TRY(X) { try { (X); } catch(std::exception *e) { printf("%s\n", e->what()); FAIL(); }}

static Program *createDummyProgram() {
    Environment *e = new Environment;
    Program *p = new Program(e);
    return p;
}

static Parser *createStringParser(String &str) {
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

TEST(Parser, ReturnStmt) {
    String in("return 0");
    Parser *parser = createStringParser(in);
    Stmt *stmt;
    TRY((stmt = parser->parseStmt()));
}

TEST(Parser, PrimativeType) {
    String in;
    Parser *parser = NULL;
    PrimativeType *type = NULL;

#define TEST_STR(EXP, STR) \
    in = String(STR); \
    parser = createStringParser(in);\
    type = parser->parsePrimativeType();\
    if(type) {\
        EXPECT_EQ(EXP, type->getKind());\
        EXPECT_EQ(true, parser->getLexer()->eof());\
    } else {\
        EXPECT_NE((Type*) NULL, type);\
    }\
    delete parser;\

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
}
