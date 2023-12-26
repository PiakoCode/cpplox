//
// Created by Piako on 23-12-24.
//

#ifndef CPPLOX_OBJECT_H
#define CPPLOX_OBJECT_H

#include "common.h"

typedef enum {
  OBJ_STRING,
} ObjType;

class Obj {
    public:
    ObjType type;
};

class ObjString {
    public:
    Obj obj;
    int length;
    char* chars;

};



#endif //CPPLOX_OBJECT_H
