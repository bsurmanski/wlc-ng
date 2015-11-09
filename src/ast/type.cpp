#include "type.hpp"

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

PrimativeType::PrimativeType(Kind _kind) : kind(_kind) {
}

PrimativeType::Kind PrimativeType::getKind() {
    return kind;
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
