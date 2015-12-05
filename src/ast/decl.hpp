#ifndef _DECL_HPP
#define _DECL_HPP

#include "common/dynarray.hpp"
#include "common/string.hpp"
#include "common/stringFormatter.hpp"
#include "type.hpp"
#include "expr.hpp"

class Decl : public Stmt {
};

class TypeDecl : public Decl {
};

class IdTypeDecl : public TypeDecl {
    String name;
    String base;
    DynArray<Decl*> members;

    virtual String serializeName() = 0;

    public:
    void setName(const String &_name);
    void setBase(const String &_base);
    void addMember(Decl* decl);
    virtual bool isStructDecl();
    virtual bool isUnionDecl();
    virtual bool isClassDecl();
    virtual bool isInterfaceDecl();
    virtual void serialize(StringFormatter &sfmt);
};

class StructDecl : public IdTypeDecl {
    virtual String serializeName();
    public:
    virtual bool isStructDecl();
};

class UnionDecl : public IdTypeDecl {
    virtual String serializeName();
    public:
    virtual bool isUnionDecl();
};

class ClassDecl : public IdTypeDecl {
    virtual String serializeName();
    public:
    virtual bool isClassDecl();
}; //TODO: class extends

class InterfaceDecl : public IdTypeDecl {
    virtual String serializeName();
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
