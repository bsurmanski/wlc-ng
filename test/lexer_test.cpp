#include <gtest/gtest.h>

#include "io/stringInput.hpp"
#include "lex/token.hpp"
#include "lex/lexer.hpp"

TEST(Lexer, Keywords) {
	Lexer *lex;
#define TEST_KW(KW) lex = new Lexer(new StringInput(#KW));\
					ASSERT_EQ(tok::kw_##KW, lex->lex().getKind());\
					delete lex;

	TEST_KW(struct);
	TEST_KW(class);
	TEST_KW(int);
	TEST_KW(uint);
	TEST_KW(long);
	TEST_KW(if);
	TEST_KW(for);
	TEST_KW(var);
	TEST_KW(include);
	TEST_KW(void);

#undef TEST_KW
}

TEST(Lexer, Integers) {
	Lexer *lex;
	Token tok;
	#define TEST_INT(NUM) lex = new Lexer(new StringInput(#NUM));\
					tok = lex->lex(); \
					ASSERT_EQ(tok::intlit, tok.getKind());\
					\
					delete lex; // TODO: assert integer value_comp
					
	TEST_INT(0);
	TEST_INT(123);
	TEST_INT(55555);
	TEST_INT(128);
	
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
}

TEST(Lexer, Chars) {
}

TEST(Lexer, Identifiers) {
}

TEST(Lexer, Punctuation) {
}