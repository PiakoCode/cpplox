//
// Created by Piako on 23-12-24.
//
#ifndef CPPLOX_OBJECT_H
#define CPPLOX_OBJECT_H

#include "common.h"

enum ObjType {
    OBJ_STRING,
};


class Obj {
public:
    ObjType type;
    struct Obj* next;
    virtual ~Obj() = default;
    explicit Obj(ObjType type):type(type){};
    
};

#endif // CPPLOX_OBJECT_H
