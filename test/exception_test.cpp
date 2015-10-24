#include <gtest/gtest.h>

#include <iostream>
#include "exception/exception.hpp"

TEST(Exception, Catch) {
    try {
        throw new Exception("Exception Thrown!");
    } catch(std::exception *e) {
        EXPECT_STREQ("Exception Thrown!", e->what());
    }
}
