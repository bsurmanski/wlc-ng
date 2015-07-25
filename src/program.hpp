#ifndef _PROGRAM_HPP
#define _PROGRAM_HPP

#include "ast/module.hpp"
#include "io/environment.hpp"
#include "common/dynarray.hpp"

class Program {
    DynArray<Module*> modules;
    Environment *env;

    public:
    Program(Environment *_env);
    ~Program();

};

#endif
