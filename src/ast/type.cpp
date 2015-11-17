#include "type.hpp"
#include "exception/exception.hpp"

static PrimativeType *boolTy = NULL;
static PrimativeType *voidTy = NULL;

static PrimativeType *int8Ty = NULL;
static PrimativeType *int16Ty = NULL;
static PrimativeType *int32Ty = NULL;
static PrimativeType *int64Ty = NULL;

static PrimativeType *uint8Ty = NULL;
static PrimativeType *uint16Ty = NULL;
static PrimativeType *uint32Ty = NULL;
static PrimativeType *uint64Ty = NULL;

static PrimativeType *float32Ty = NULL;
static PrimativeType *float64Ty = NULL;

Type::Type() : ptrTy(NULL) {
}

Type::~Type() {
    if(ptrTy) {
        ptrTy->release();
    }
}

PointerType *Type::getPointer() {
    if(!ptrTy) {
        ptrTy = new PointerType(this);
    }
    return ptrTy;
}

TupleType *Type::tupleWith(Type *ty) {
    if(ty->isTupleType()) {
        //TODO: unwrap tuple
    }

    //TODO: look up in map the correct tupling with ty; will form linked-list

    throw Exception("unimplemented: tupling");
}

String Type::serialized() {
    StringFormatter sfmt;
    serialize(sfmt);
    return sfmt.toString();
}

PrimativeType *Type::asPrimativeType() {
    return NULL;
}

TupleType *Type::asTupleType() {
    return NULL;
}

FunctionType *Type::asFunctionType() {
    return NULL;
}

PointerType *Type::asPointerType() {
    return NULL;
}

ArrayType *Type::asArrayType() {
    return NULL;
}

UserType *Type::asUserType() {
    return NULL;
}

bool Type::isPrimativeType() {
    return asPrimativeType();
}

bool Type::isTupleType() {
    return asTupleType();
}

bool Type::isFunctionType() {
    return asFunctionType();
}

bool Type::isPointerType() {
    return asPointerType();
}

bool Type::isArrayType() {
    return asArrayType();
}

bool Type::isUserType() {
    return asUserType();
}

PrimativeType::PrimativeType(Kind _kind) : kind(_kind) {
}

PrimativeType::Kind PrimativeType::getKind() {
    return kind;
}

void PrimativeType::serialize(StringFormatter &sfmt) {
    String str;
    switch(kind) {
        case BOOL:
            str = "bool";
            break;
        case VOID:
            str = "void";
            break;
        case INT8:
            str = "int8";
            break;
        case INT16:
            str = "int16";
            break;
        case INT32:
            str = "int32";
            break;
        case INT64:
            str = "int64";
            break;
        case UINT8:
            str = "uint8";
            break;
        case UINT16:
            str = "uint16";
            break;
        case UINT32:
            str = "uint32";
            break;
        case UINT64:
            str = "uint64";
            break;
        case FLOAT32:
            str = "float32";
            break;
        case FLOAT64:
            str = "float64";
            break;
        default:
            throw Exception("invalid type");
    }
    sfmt.write(str);
}

PrimativeType *PrimativeType::getBool() {
    if(!boolTy) boolTy = new PrimativeType(PrimativeType::BOOL);
    return boolTy;
}

PrimativeType *PrimativeType::getVoid() {
    if(!voidTy) voidTy = new PrimativeType(PrimativeType::VOID);
    return voidTy;
}

PrimativeType *PrimativeType::getInt8() {
    if(!int8Ty) int8Ty = new PrimativeType(PrimativeType::INT8);
    return int8Ty;
}

PrimativeType *PrimativeType::getInt16() {
    if(!int16Ty) int16Ty = new PrimativeType(PrimativeType::INT16);
    return int16Ty;
}

PrimativeType *PrimativeType::getInt32() {
    if(!int32Ty) int32Ty = new PrimativeType(PrimativeType::INT32);
    return int32Ty;
}

PrimativeType *PrimativeType::getInt64() {
    if(!int64Ty) int64Ty = new PrimativeType(PrimativeType::INT64);
    return int64Ty;
}

PrimativeType *PrimativeType::getUInt8() {
    if(!uint8Ty) uint8Ty = new PrimativeType(PrimativeType::UINT8);
    return uint8Ty;
}

PrimativeType *PrimativeType::getUInt16() {
    if(!uint16Ty) uint16Ty = new PrimativeType(PrimativeType::UINT16);
    return uint16Ty;
}

PrimativeType *PrimativeType::getUInt32() {
    if(!uint32Ty) uint32Ty = new PrimativeType(PrimativeType::UINT32);
    return uint32Ty;
}

PrimativeType *PrimativeType::getUInt64() {
    if(!uint64Ty) uint64Ty = new PrimativeType(PrimativeType::UINT64);
    return uint64Ty;
}

PrimativeType *PrimativeType::getFloat32() {
    if(!float32Ty) float32Ty = new PrimativeType(PrimativeType::FLOAT32);
    return float32Ty;
}

PrimativeType *PrimativeType::getFloat64() {
    if(!float64Ty) float64Ty = new PrimativeType(PrimativeType::FLOAT64);
    return float64Ty;
}

PointerType::PointerType(Type *ty) : baseType(ty) {
}

void PointerType::serialize(StringFormatter &sfmt) {
    sfmt.write("(ptr ");
    baseType->serialize(sfmt);
    sfmt.write(")");
}

UserType::UserType(const String &_name) : name(_name) {
}

void UserType::serialize(StringFormatter &sfmt) {
    sfmt.write("(type ");
    sfmt.write(name);
    sfmt.write(")");
}
