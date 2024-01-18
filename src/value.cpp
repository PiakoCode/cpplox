//
// Created by Piako on 23-12-8.
//
#include "value.h"
#include <cstdio>

void printObject(Value value)
{
    switch (value.asObj()->type) {

    case OBJ_STRING:
        std::printf("%s", value.asString()->c_str());
        break;
    }
}

void printValue(Value value)
{
    switch (value.getType()) {

    case ValueType::Bool:
        if (value.asBool()) {
            printf("true");
        } else {
            printf("false");
        }
        break;

    case ValueType::Nil:
        printf("Nil");
        break;

    case ValueType::Number:
        printf("%g", value.asNumber());
        break;
    case ValueType::Obj:
        printObject(value);
    default:
        return; // unreachable
    }
}
