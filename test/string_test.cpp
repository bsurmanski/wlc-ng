#include <gtest/gtest.h>
#include <stdio.h>

#include "common/string.hpp"

TEST(String, Create) {
	String str("Hello");
	EXPECT_FALSE(memcmp(str.c_str(), "Hello", 5));
}

TEST(String, Compare) {
	String str1("SomeString");
	String str2("SomeString");
	String str3("somestring");
	
	String lstr1("123456789012345678901234567890123456789");
	String lstr2("123456789012345678901234567890123456789");
	String lstr3("123456789012345678901234567890123456781");
	EXPECT_EQ(str1, str2);
	EXPECT_NE(str1, str3);
	EXPECT_EQ(lstr1, lstr2);
	EXPECT_NE(lstr1, lstr3);
	
	String str4("aaab");
	String str5("aaac");
	String str6("aaaca");
	EXPECT_LT(str4, str5);
	EXPECT_GT(str5, str4);
	EXPECT_LT(str5, str6);
}

TEST(String, Length) {
	String str1("123");
	String str2("Hello");
	String str3("Here is a long string. it is intended to be longer than 32 characters.");
	String str4;
	EXPECT_EQ(str1.length(), 3);
	EXPECT_EQ(str2.length(), 5);
	EXPECT_EQ(str3.length(), 70);
	EXPECT_EQ(str4.length(), 0);
}

TEST(String, Append) {
	String str1;
	str1.append('a');
	EXPECT_EQ(str1, "a");
	str1.append("lways");
	EXPECT_EQ(str1, "always"); 
	str1.append(" bet on black. always and forever");
	EXPECT_EQ(str1, "always bet on black. always and forever");
}

TEST(String, Cstr) {
	String str1;
	str1.append("1234567890");
	EXPECT_STREQ(str1.c_str(), "1234567890");
	str1.append("123456789012345678901234567890");
	EXPECT_STREQ(str1.c_str(), "1234567890123456789012345678901234567890");
}

TEST(String, Empty) {
	String str1;
	EXPECT_TRUE(str1.empty());
	str1.append('\0');
	EXPECT_FALSE(str1.empty());
}

TEST(String, Copy) {
	char cstr1[32];
	String str1("Hello");
	str1.copy(cstr1, 5);
	cstr1[6] = '\0';
	EXPECT_STREQ(cstr1, "Hello");
	str1.copy(cstr1, 2, 2);
	EXPECT_STREQ(cstr1, "llllo");
}

TEST(String, Dup) {
	String st1("test");
	String st2 = st1;
	String st3 = st1.dup();

	EXPECT_EQ(st1, st2);
	EXPECT_EQ(st1, st3);
	st1[0] = 'b';
	EXPECT_NE(st1, st3);
	
	String st4("1234567890123456789012345678901234567890");
	String st5 = st4;
	String st6 = st4.dup();
	st4[0] = '0';
	EXPECT_EQ(st4, st5);
	EXPECT_NE(st4, st6);
}