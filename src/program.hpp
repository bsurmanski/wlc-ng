#ifndef _PROGRAM_HPP
#define _PROGRAM_HPP

#include <vector>

#include "ast/module.hpp"
#include "io/environment.hpp"

class Program {
    std::vector<Module*> modules;
    Environment *env;

    public:
    Program(Environment *_env);

};

#endif
