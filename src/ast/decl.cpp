#include "decl.hpp"

void UserTypeDecl::setName(String _name) {
    name = _name;
}

bool UserTypeDecl::isStructDecl() {
    return false;
}

bool UserTypeDecl::isUnionDecl() {
    return false;
}

bool UserTypeDecl::isClassDecl() {
    return false;
}

bool UserTypeDecl::isInterfaceDecl() {
    return false;
}


bool StructDecl::isStructDecl() {
    return true;
}

bool UnionDecl::isUnionDecl() {
    return true;
}

bool ClassDecl::isClassDecl() {
    return true;
}

bool InterfaceDecl::isInterfaceDecl() {
    return true;
}


VarDecl::VarDecl(Type *_type, const String &_name, Expr *_defaultValue) : type(_type), name(_name), defaultValue(_defaultValue) {
}

VarDecl::~VarDecl() {
    //type->release();
    delete defaultValue;
}

void VarDecl::serialize(StringFormatter &sfmt) {
    sfmt.write("(var ");
    sfmt.write(name);
    sfmt.write(" ");
    type->serialize(sfmt);
    sfmt.write(")");
}

FuncDecl::FuncDecl(Type *_returnType, const String &_name, DynArray<VarDecl*> _params, Stmt *_body) : returnType(_returnType), name(_name), params(_params), body(_body) {
}

FuncDecl::~FuncDecl() {
    while(!params.empty()) {
        delete params.pop();
    }

    delete body;
}

void FuncDecl::serialize(StringFormatter &sfmt) {
    sfmt.write("(func ");
    sfmt.write(name);
    sfmt.write(" ");
    returnType->serialize(sfmt);
    sfmt.write(" ");

    sfmt.write("(vars");
    for(int i = 0; i < params.size(); i++) {
        sfmt.write(" ");
        params[i]->serialize(sfmt);
    }
    sfmt.write(")");

    sfmt.newline();
    sfmt.indent();
    body->serialize(sfmt);
    sfmt.unindent();
    sfmt.write(")");
}
