#include <gtest/gtest.h>

#include "program.hpp"
#include "io/environment.hpp"
#include "io/stringInput.hpp"
#include "common/string.hpp"
#include "parse/parser.hpp"

static Program *createDummyProgram() {
    Environment *e = new Environment;
    Program *p = new Program(e);
    return p;
}

static Parser *createStringParser(String &str) {
    Program *prog = createDummyProgram();
    Lexer *lex = new Lexer(new StringInput(str));
    return new Parser(prog, lex);
}

TEST(Parser, ReturnStmt) {
    String in("return 0");
    Parser *parser = createStringParser(in);
}
