#include <gtest/gtest.h>

#include "io/stringInput.hpp"

TEST(StringInput, Read) {
	String s("Hello Test");
	StringInput input(s);
	for(int i = 0; i < s.length(); i++) {
		ASSERT_EQ(s[i], input.get());
	}
	
	ASSERT_TRUE(input.eof());
}