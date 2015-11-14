#include <gtest/gtest.h>

#include <iostream>
#include "exception/exception.hpp"
#include "exception/parseException.hpp"
#include "io/stringInput.hpp"

TEST(Exception, CatchException) {
    try {
        throw Exception("Exception Thrown!");
    } catch(std::exception &e) {
        EXPECT_STREQ("Exception Thrown!", e.what());
    }
}

TEST(Exception, CatchParseException) {
    StringInput *si = new StringInput("hello");
    try {
        throw ParseException(SourceLocation(si, 1), "Exception Thrown!");
    } catch(std::exception &e) {
        EXPECT_STREQ("string-constant:1:1: Exception Thrown!", e.what());
    }
    delete  si;
}
