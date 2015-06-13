#ifndef _PROGRAM_HPP
#define _PROGRAM_HPP

#include "ast/module.hpp"
#include "io/environment.hpp"
#include "common/varray.hpp"

class Program {
    VArray<Module*> modules;
    Environment *env;

    public:
    Program(Environment *_env);

};

#endif
