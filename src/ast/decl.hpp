#ifndef _DECL_HPP
#define _DECL_HPP

#include "common/dynarray.hpp"
#include "common/string.hpp"
#include "common/stringFormatter.hpp"
#include "type.hpp"
#include "expr.hpp"

class Decl : public Expr {
};

class TypeDecl : public Decl {
};

class UserTypeDecl : public TypeDecl {
    String name;

    public:
    void setName(String _name);
    virtual bool isStructDecl();
    virtual bool isUnionDecl();
    virtual bool isClassDecl();
    virtual bool isInterfaceDecl();
};

class StructDecl : public UserTypeDecl {
    public:
    virtual bool isStructDecl();
};

class UnionDecl : public UserTypeDecl {
    public:
    virtual bool isUnionDecl();
};

class ClassDecl : public UserTypeDecl {
    public:
    virtual bool isClassDecl();
};

class InterfaceDecl : public UserTypeDecl {
    public:
    virtual bool isInterfaceDecl();
};

class AliasDecl : public Decl {
};

class VarDecl : public Decl {
    Type *type;
    String name;
    Expr *defaultValue;

    public:
    VarDecl(Type *_type, const String &_name, Expr *_defaultValue);
    ~VarDecl();
    virtual void serialize(StringFormatter &sfmt);
};

class FuncDecl : public Decl {
    Type *returnType;
    String name;
    DynArray<VarDecl*> params;
    Stmt *body;

    public:
    FuncDecl(Type *_returnType, const String &_name, DynArray<VarDecl*> _params, Stmt *_body);
    ~FuncDecl();
    virtual void serialize(StringFormatter &sfmt);
};

#endif
