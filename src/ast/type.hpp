#ifndef _TYPE_HPP
#define _TYPE_HPP

#include "common/dynarray.hpp"
#include "common/object.hpp"
#include "common/string.hpp"
#include "common/stringFormatter.hpp"

class PrimativeType;
class TupleType;
class FunctionType;
class PointerType;
class ArrayType;
class UserType;

class Type : public Object {
    PointerType *ptrTy;

    public:
    Type();
    virtual ~Type();
    PointerType *getPointer();
    TupleType *tupleWith(Type *ty);
    virtual String serialized();
    virtual void serialize(StringFormatter &sfmt) = 0;


    virtual PrimativeType   *asPrimativeType();
    virtual TupleType       *asTupleType();
    virtual FunctionType    *asFunctionType();
    virtual PointerType     *asPointerType();
    virtual ArrayType       *asArrayType();
    virtual UserType        *asUserType();

    virtual bool isPrimativeType();
    virtual bool isTupleType();
    virtual bool isFunctionType();
    virtual bool isPointerType();
    virtual bool isArrayType();
    virtual bool isUserType();
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
    virtual void serialize(StringFormatter &sfmt);

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

class TupleType : public Type {
    DynArray<Type*> memberTypes;
};

class FunctionType : public Type {
    Type *returnType;
    DynArray<Type*> paramTypes;
};

class PointerType : public Type {
    Type *baseType;

    public:
    PointerType(Type *ty);
    virtual void serialize(StringFormatter &sfmt);
};

class ArrayType : public Type {
    Type *baseType;
};

class StaticArrayType : public ArrayType {
    int length;
};

class DynamicArrayType : public ArrayType {
};

class UserType : public Type {
    String name;
    public:
    UserType(const String &_name);
    virtual void serialize(StringFormatter &sfmt);
};

#endif
