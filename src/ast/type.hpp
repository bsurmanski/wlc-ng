#ifndef _TYPE_HPP
#define _TYPE_HPP

#include "common/dynarray.hpp"
#include "common/object.hpp"

class Type : public Object {
    public:
};

class PrimativeType : public Type {
    public:
    enum Kind {
        BOOL,
        VOID,
        INT8,
        INT16,
        INT32,
        INT64,
        UINT8,
        UINT16,
        UINT32,
        UINT64,
        FLOAT32,
        FLOAT64
    };

    protected:
    Kind kind;

    public:
    PrimativeType(Kind _kind);
    Kind getKind();

    static PrimativeType *getBool();
    static PrimativeType *getVoid();
    static PrimativeType *getInt8();
    static PrimativeType *getInt16();
    static PrimativeType *getInt32();
    static PrimativeType *getInt64();
    static PrimativeType *getUInt8();
    static PrimativeType *getUInt16();
    static PrimativeType *getUInt32();
    static PrimativeType *getUInt64();
    static PrimativeType *getFloat32();
    static PrimativeType *getFloat64();
};

class UserType : public Type {
    DynArray<Type*> memberTypes;
};

class TupleType : public Type {
    DynArray<Type*> memberTypes;
};

class FunctionType : public Type {
    Type *returnType;
    TupleType *paramType;
};

class PointerType : public Type {
    Type *baseType;
};

class ArrayType : public Type {
    Type *baseType;
};

class StaticArrayType : public ArrayType {
    int length;
};

class DynamicArrayType : public ArrayType {
};

#endif
