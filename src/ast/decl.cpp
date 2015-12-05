#include "decl.hpp"

void IdTypeDecl::setName(const String &_name) {
    name = _name;
}
void IdTypeDecl::setBase(const String &_base) {
    base= _base;
}

void IdTypeDecl::addMember(Decl* decl) {
    members.append(decl);
}

bool IdTypeDecl::isStructDecl() {
    return false;
}

bool IdTypeDecl::isUnionDecl() {
    return false;
}

bool IdTypeDecl::isClassDecl() {
    return false;
}

bool IdTypeDecl::isInterfaceDecl() {
    return false;
}

void IdTypeDecl::serialize(StringFormatter &sfmt) {
    sfmt.write("(");
    sfmt.write(serializeName());
    sfmt.write(" ");
    sfmt.write(name);
    if(!base.empty()) {
        sfmt.write(" ");
        sfmt.write("(id ");
        sfmt.write(base);
        sfmt.write(")");
    }
    sfmt.indent();
    sfmt.write(" (decls");
    for(int i = 0; i < members.size(); i++) {
        sfmt.newline();
        members[i]->serialize(sfmt);
    }
    sfmt.unindent();
    sfmt.write(")");
    sfmt.write(")");
}

bool StructDecl::isStructDecl() {
    return true;
}

String StructDecl::serializeName() {
    return "struct";
}

bool UnionDecl::isUnionDecl() {
    return true;
}

String UnionDecl::serializeName() {
    return "union";
}

bool ClassDecl::isClassDecl() {
    return true;
}

String ClassDecl::serializeName() {
    return "class";
}

bool InterfaceDecl::isInterfaceDecl() {
    return true;
}

String InterfaceDecl::serializeName() {
    return "class";
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
    if(defaultValue) {
        sfmt.write(" ");
        defaultValue->serialize(sfmt);
    }
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
