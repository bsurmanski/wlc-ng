#ifndef _DECL_HPP
#define _DECL_HPP

#include "expr.hpp"

class Decl : Expr {
};

class TypeDecl : public Decl {
};

class UserTypeDecl : public TypeDecl {
};

class StructDecl : public UserTypeDecl {
};

class UnionDecl : public UserTypeDecl {
};

class ClassDecl : public UserTypeDecl {
};

class InterfaceDecl : public UserTypeDecl {
};

class AliasDecl : public Decl {
};

class FuncDecl : public Decl {
};

class VarDecl : public Decl {
};

#endif
