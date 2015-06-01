#ifndef _PARSER_HPP
#define _PARSER_HPP

#include "../program.hpp"
#include "../lex/lexer.hpp"
#include "../ast/module.hpp"

class Parser {
    Program *program;
    Lexer *lex;

    Module *module;

    public:
    Parser(Program *_program, Lexer *_lex);
    Module *parse();
};

#endif
