#include <gtest/gtest.h>

#include "common/buffer.hpp"

TEST(Buffer, Add) {
    Buffer<int> buf;
    buf.push(5);
    ASSERT_EQ(5, buf.pop());

    buf.push(1);
    buf.push(2);
    buf.push(3);
    ASSERT_EQ(1, buf.pop());
    ASSERT_EQ(2, buf.pop());
    ASSERT_EQ(3, buf.pop());
}

TEST(Buffer, Loop) {
    Buffer<int> buf;

    for(int i = 0; i < 15; i++) {
        buf.push(i);
    }

    for(int i = 0; i < 15; i++) {
        ASSERT_EQ(i, buf.pop());
    }

    for(int i = 0; i < 15; i++) {
        buf.push(i);
    }

    for(int i = 0; i < 15; i++) {
        ASSERT_EQ(i, buf.pop());
    }
}

TEST(Buffer, Length) {
    Buffer<int> buf;
    for(int i = 0; i < 15; i++) {
        buf.push(i);
        ASSERT_EQ(i+1, buf.length());
    }

    buf.clear();
}
