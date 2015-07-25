#include "program.hpp"

Program::Program(Environment *_env) : env(_env) {
}

Program::~Program() {
    delete env;
}
