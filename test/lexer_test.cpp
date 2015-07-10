#include <gtest/gtest.h>

#include "io/stringInput.hpp"
#include "lex/token.hpp"
#include "lex/lexer.hpp"
#include "exception/exception.hpp"

TEST(Lexer, Keywords) {
	Lexer *lex;
#define TEST_KW(KW) lex = new Lexer(new StringInput(#KW));\
					ASSERT_EQ(tok::kw_##KW, lex->lex().getKind());\
					ASSERT_EQ(lex->eof(), true); \
					delete lex;

// run 'TEST_KW' on all defined keywords
#define KEYWORD(X) TEST_KW(X);
#include "lex/tokenkinds.def"
#undef KEYWORD
#undef TEST_KW
}

TEST(Lexer, Integers) {
	Lexer *lex;
	Token tok;
	#define TEST_INT(EXP, NUM) lex = new Lexer(new StringInput(#NUM));\
					tok = lex->lex(); \
					ASSERT_EQ(tok::intlit, tok.getKind());\
					ASSERT_EQ(EXP, tok.getUIntData());\
					delete lex;

	TEST_INT(0, 0);
	TEST_INT(123, 123);
	TEST_INT(55555, 55555);
	TEST_INT(128, 128);
	TEST_INT(145, 1_4_5);
	TEST_INT(1234, 0_1_2___3___4);
	TEST_INT(0xB, 0xB);
	TEST_INT(0x10, 0x10);
	TEST_INT(0xF0, 0b11110000);
	TEST_INT(060, 0o60);
	TEST_INT(0xFFFFFFFF, 0xFFFFFFFF);

	#undef TEST_INT
}

TEST(Lexer, Strings) {
	Lexer *lex;
	Token tok;

	#define TEST_STR(STR) lex = new Lexer(new StringInput(#STR));\
					tok = lex->lex();\
					ASSERT_EQ(tok::stringlit, tok.getKind());\
					ASSERT_EQ(tok.getStringData(), STR);\
					delete lex;

	TEST_STR("Hello");
	TEST_STR("Hi\"");
	TEST_STR("Long String with \' escape \' characters \t \n");

	#undef TEST_STR
	#define TEST_STR(EXP, STR) lex = new Lexer(new StringInput(STR));\
					tok = lex->lex();\
					ASSERT_EQ(tok::stringlit, tok.getKind());\
					ASSERT_EQ(tok.getStringData(), EXP);\
					delete lex;
    TEST_STR("empty escape sequence", "\"empty\\  escape\\  sequence\"");
	TEST_STR("raw string \\ :)", "`raw string \\ :)`");
    TEST_STR("raw string\n with newlines", "`raw string\n with newlines`");

	#undef TEST_STR
}

TEST(Lexer, EscapeStrings) {
	Lexer *lex;
	Token tok;
	#define TEST_STR(EXP, STR) lex = new Lexer(new StringInput(STR));\
					tok = lex->lex();\
					ASSERT_EQ(tok::stringlit, tok.getKind());\
					ASSERT_EQ(tok.getStringData(), EXP);\
					delete lex;

    // hex
    TEST_STR("\x45", "\"\\x45\"");
    TEST_STR("\x12\x34\x56", "\"\\x123456\"");
    TEST_STR("\x12\x34\x50", "\"\\x12345\"");
    TEST_STR("\x12\x34\x55hex", "\"\\x123455hex\"");

    // spacing
    TEST_STR("\x12\x34\x50""123", "\"\\x12345\\ 123\"");

    // binary
    TEST_STR("\xff", "\"\\b11111111\"");
    TEST_STR("\xe0", "\"\\b111\"");

    // octal
    TEST_STR("\xff\xff\xff", "\"\\o77777777\"");
    TEST_STR("\xff\x80", "\"\\o777\"");

    //UTF8
    TEST_STR("\u0070", "\"\\u70\"");
    TEST_STR("\u007F", "\"\\u7F\"");
    TEST_STR("\u0080", "\"\\u80\"");
    TEST_STR("\u0280", "\"\\u280\"");
    TEST_STR("\u07ff", "\"\\u7ff\"");
    TEST_STR("\u0880", "\"\\u880\"");
    TEST_STR("\U0010FFFF", "\"\\u10ffff\"");

    #undef TEST_STR
}

TEST(Lexer, Floats) {
	Lexer *lex;
	Token tok;

	#define TEST_FLOAT(EXP, NUM) lex = new Lexer(new StringInput(#NUM));\
				tok = lex->lex(); \
				ASSERT_EQ(tok::floatlit, tok.getKind());\
				ASSERT_EQ(EXP, tok.getFloatData());\
				delete lex;

	TEST_FLOAT(1.0, 1.0);
	TEST_FLOAT(2.5, 2.5);
	TEST_FLOAT(3.141592654, 3.141592654);
	TEST_FLOAT(1655.123456, 1_655.123_456);
	TEST_FLOAT(0.0, 0_.000);
	TEST_FLOAT(0x1.1p0, 0x1.1);
	TEST_FLOAT(0x1.11p0, 0x1.11);
	TEST_FLOAT(0.75, 0b0.11);
	TEST_FLOAT(0.75, 0b0_.1_1);
	TEST_FLOAT(1.0E10, 1.0E10);
	TEST_FLOAT(0x1.0p10, 1p10);
	TEST_FLOAT(0.1, 0.1);
	TEST_FLOAT(10E-10, 10e-10);
	TEST_FLOAT(0x1p-10, 1P-10);

	//TODO: test float epsilon and float limits

	#undef TEST_FLOAT
}

TEST(Lexer, Chars) {
	Lexer *lex;
	Token tok;

	#define TEST_CHAR(C) lex = new Lexer(new StringInput(#C));\
					tok = lex->lex();\
					ASSERT_EQ(tok::charlit, tok.getKind());\
					ASSERT_EQ(C, tok.getUIntData());\
					delete lex;

	TEST_CHAR('a');
	TEST_CHAR('0');
	TEST_CHAR('\n');
	TEST_CHAR('@');
	TEST_CHAR('!');
	TEST_CHAR('/');

	#undef TEST_CHAR
}

TEST(Lexer, Identifiers) {
	Lexer *lex;
	Token tok;

	#define TEST_ID(ID) lex = new Lexer(new StringInput(#ID));\
					tok = lex->lex();\
					ASSERT_EQ(tok::identifier, tok.getKind());\
					ASSERT_EQ(tok.getStringData(), #ID);\
					delete lex;

	TEST_ID(hello);
	TEST_ID(something);
	TEST_ID(i);
	TEST_ID(int_);
	TEST_ID(_int);
	TEST_ID(thing5);
	TEST_ID(word_123);

	#undef TEST_ID

	#define TEST_ID(EXP, ID) lex = new Lexer(new StringInput(#ID));\
					tok = lex->lex();\
					ASSERT_EQ(tok::identifier, tok.getKind());\
					ASSERT_EQ(tok.getStringData(), #EXP);\
					delete lex;

	TEST_ID(int_, int_+1);
	TEST_ID(id, id!@#);

	#undef TEST_ID
}

TEST(Lexer, Punctuation) {
	Lexer *lex;
	Token tok;

#define TEST_PUNCT(TOK,P) lex = new Lexer(new StringInput(P));\
					tok = lex->lex();\
					ASSERT_EQ(tok::TOK, tok.getKind());\
					ASSERT_EQ(lex->eof(), true); \
					delete lex;

// run 'TEST_PUNCT' on all defined punctuation
#define PUNCT(X,Y) TEST_PUNCT(X, Y);
#include "lex/tokenkinds.def"
#undef PUNCT
#undef TEST_PUNCT
}

TEST(Lexer, Comments) {
	Lexer *lex;
	Token tok;
	#define TEST_COMMENT(EXP, STR) lex = new Lexer(new StringInput(STR));\
					tok = lex->lexRaw();\
					ASSERT_EQ(tok::comment, tok.getKind());\
					ASSERT_EQ(tok.getStringData(), EXP);\
					delete lex;

	TEST_COMMENT("hello comment", "/*hello comment*/");
	TEST_COMMENT("nested /*comment*/ ", "/*nested /*comment*/ */");
	//TEST_COMMENT("nested /*comment 2*/", "/*nested /*comment 2*/*/");
	TEST_COMMENT("line comment", "//line comment");
	TEST_COMMENT("newline comment", "//newline comment\n //with extra tokens");

	#undef TEST_COMMENT
}

TEST(Lexer, MultiTokens) {
    String test("int i = 5\nint j = 6");

    Lexer *lex = new Lexer(new StringInput(test));
    ASSERT_EQ(tok::kw_int, lex->lex().getKind());
    ASSERT_EQ(tok::identifier, lex->lex().getKind());
    ASSERT_EQ(tok::equal, lex->lex().getKind());
    ASSERT_EQ(tok::intlit, lex->lex().getKind());
    ASSERT_EQ(tok::newline, lex->lex().getKind());

    ASSERT_EQ(tok::kw_int, lex->lex().getKind());
    ASSERT_EQ(tok::identifier, lex->lex().getKind());
    ASSERT_EQ(tok::equal, lex->lex().getKind());
    ASSERT_EQ(tok::intlit, lex->lex().getKind());
    ASSERT_EQ(true, lex->eof());
    delete lex;
}
