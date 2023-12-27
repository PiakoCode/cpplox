//
// Created by Piako on 23-12-24.
//
#ifndef CPPLOX_OBJECT_H
#define CPPLOX_OBJECT_H

#include "common.h"
#include <cstring>
#include <string>

enum ObjType {
    OBJ_STRING,
};

class Obj {
public:
    ObjType type;
    ~Obj() = default;
};

class ObjString : public Obj {
public:
    std::string string;

    explicit ObjString(const char* str)
    {
        this->type = OBJ_STRING;
        this->string = std::string(str);
    }

    explicit ObjString(const std::string& str)
    {
        this->type = OBJ_STRING;
        this->string = std::string(str);
    }

    static ObjString* copyString(const std::string& str)
    {
        auto* p = new ObjString(str.c_str());
        return p;
    }
};

#endif // CPPLOX_OBJECT_H
