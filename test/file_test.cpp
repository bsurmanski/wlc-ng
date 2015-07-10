#include <gtest/gtest.h>

#include "io/file.hpp"

TEST(File, Get) {
    File *f = new File("file_test_input");
    const char *str = "#include";
    for(int i = 0; i < strlen(str); i++) {
        ASSERT_EQ(str[i], f->get());
    }
    delete f;
}

TEST(File, Peek) {
    File *f = new File("file_test_input");
    ASSERT_EQ('#', f->peek());
    ASSERT_EQ('#', f->peek());
    ASSERT_EQ('i', f->peek(1));
    ASSERT_EQ('i', f->peek(1));
    ASSERT_EQ('e', f->peek(7));
    ASSERT_EQ('e', f->peek(7));
    ASSERT_EQ('#', f->peek());
    ASSERT_EQ('#', f->get());
    delete f;
}

TEST(File, Read) {
    File *f = new File("file_test_input");
    const char *str = "#include";
    char buf[128];
    f->read(buf, 1, 8);
    for(int i = 0; i < 8; i++) {
        ASSERT_EQ(buf[i], str[i]);
    }

    ASSERT_EQ(' ', f->get());
    ASSERT_EQ('<', f->get());
}

TEST(File, Seek) {
    File *f = new File("file_test_input");
    f->seek(9);
    ASSERT_EQ('<', f->get());
    ASSERT_EQ('s', f->get());
    f->seek(6);
    ASSERT_EQ('>', f->get());
    f->seek(-3);
    ASSERT_EQ('.', f->get());
    f->set(0);
    ASSERT_EQ('#', f->get());
    delete f;
}

TEST(File, Exists) {
    ASSERT_EQ(true, File::exists("file_test_input"));
    ASSERT_EQ(false, File::exists("file_test_garbage_doesnotexist"));
}
