#ifndef _PROGRAM_HPP
#define _PROGRAM_HPP

#include "common/dynarray.hpp"
#include "common/object.hpp"

class Module;
class Environment;

class Program : public Object {
    DynArray<Module*> modules;
    Environment *env;

    public:
    Program(Environment *_env);
    ~Program();

};

#endif
