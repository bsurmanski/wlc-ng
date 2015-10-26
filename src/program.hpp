#ifndef _PROGRAM_HPP
#define _PROGRAM_HPP

#include "ast/module.hpp"
#include "io/environment.hpp"
#include "common/dynarray.hpp"
#include "common/object.hpp"

class Program : public Object {
    DynArray<Module*> modules;
    Environment *env;

    public:
    Program(Environment *_env);
    ~Program();

};

#endif
