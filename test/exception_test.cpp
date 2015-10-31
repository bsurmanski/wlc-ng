#include <gtest/gtest.h>

#include <iostream>
#include "exception/exception.hpp"
#include "exception/parseException.hpp"
#include "io/stringInput.hpp"

TEST(Exception, CatchException) {
    try {
        throw new Exception("Exception Thrown!");
    } catch(std::exception *e) {
        EXPECT_STREQ("Exception Thrown!", e->what());
    }
}

TEST(Exception, CatchParseException) {
    try {
        throw new ParseException(SourceLocation(new StringInput("hello"), 1), "Exception Thrown!");
    } catch(std::exception *e) {
        EXPECT_STREQ("string-constant:1:1: Exception Thrown!", e->what());
    }
}
