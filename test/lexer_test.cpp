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
	TEST_STR("Long String with \' escape \' characters \t \n \a \b");
	
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
	TEST_CHAR('\a');
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