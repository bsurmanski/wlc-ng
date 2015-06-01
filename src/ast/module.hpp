#ifndef _MODULE_HPP
#define _MODULE_HPP

#include "ast.hpp"

class Module {
    Module();

    void addDecl(Decl *decl);
    void addImport(Module *m);
};

#endif
