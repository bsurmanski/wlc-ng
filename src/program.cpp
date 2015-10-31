#include "program.hpp"
#include "ast/module.hpp"
#include "io/environment.hpp"

Program::Program(Environment *_env) : env(_env) {
}

Program::~Program() {
    delete env;
}
