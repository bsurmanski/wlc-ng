#include <gtest/gtest.h>

#include "common/stringFormatter.hpp"

TEST(StringFormatter, Write) {
    StringFormatter pstr;

    pstr.write("hello");
    EXPECT_EQ(String("hello"), pstr.toString());

    pstr.write(" world");
    EXPECT_EQ(String("hello world"), pstr.toString());

    pstr.write("\neveryone");
    EXPECT_EQ(String("hello world\neveryone"), pstr.toString());

    pstr.write("\n\neverywhere");
    EXPECT_EQ(String("hello world\neveryone\n\neverywhere"), pstr.toString());

    pstr.write("\n\n\n");
    EXPECT_EQ(String("hello world\neveryone\n\neverywhere\n\n\n"), pstr.toString());
}

TEST(StringFormatter, Indent) {
    StringFormatter pstr;

    pstr.write("hello");
    pstr.indent();
    pstr.write("\nworld");

    EXPECT_EQ(String("hello\n  world"), pstr.toString());

    pstr.write("\n");
    EXPECT_EQ(String("hello\n  world\n"), pstr.toString());

    pstr.write("everyone");
    EXPECT_EQ(String("hello\n  world\n  everyone"), pstr.toString());

    pstr.write("\nand all");
    EXPECT_EQ(String("hello\n  world\n  everyone\n  and all"), pstr.toString());

    pstr.reset();
    pstr.write("hello\n\n");
    pstr.indent();
    pstr.indent();
    pstr.write("world");
    pstr.indent();
    EXPECT_EQ(String("hello\n\n    world"), pstr.toString());
}

TEST(StringFormatter, UnIndent) {
    StringFormatter pstr;

    pstr.write("hello\n");
    pstr.indent();
    pstr.write("world\n");
    pstr.unindent();
    pstr.write("and all");
    EXPECT_EQ(String("hello\n  world\nand all"), pstr.toString());
}

TEST(StringFormatter, Newline) {
    StringFormatter pstr;
    pstr.write("hello");
    pstr.newline();
    pstr.indent();
    pstr.write("world");
    EXPECT_EQ(String("hello\n  world"), pstr.toString());
}
