#include "parser.hpp"

Parser::Parser(Program *_program, Lexer *_lex) {
    program = _program;
    lex  = _lex;
}

Module *Parser::parse() {
    Module *module = new Module;

    return module;
}
