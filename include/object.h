//
// Created by Piako on 23-12-24.
//
#ifndef CPPLOX_OBJECT_H
#define CPPLOX_OBJECT_H

#include "common.h"
#include "object/String.h"
#include <cstring>

enum ObjType {
    OBJ_STRING,
};


class Obj {
public:
    ObjType type;
    virtual ~Obj() = default;
};

class ObjString : public Obj {
public:
    obj::String string;
    
    explicit ObjString(const char* str)
    {
        this->type = OBJ_STRING;
        this->string = obj::String(str);
    }

    explicit ObjString(const obj::String& str)
    {
        this->type = OBJ_STRING;
        this->string = str;
    }

    static ObjString* copyString(const std::string& str)
    {
        auto *p = new ObjString(str.c_str());
        p->string.m_size = str.length();
        return p;
    }
};

#endif // CPPLOX_OBJECT_H
