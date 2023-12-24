//
// Created by Piako on 23-12-8.
//
#include "../include/value.h"
#include <cstdio>

void printValue(Value value) {
    switch (value.type) {

    case ValueType::Bool:
        if (value.asBool() == true) {
            printf("true");
        } else {
            printf("false");
        }
        break;

    case ValueType::Nil:
        printf("Nil");
        break;

    case ValueType::Number:
        printf("%g",value.asNumber());
        break;

    default:
        return; // unreachable
    }
}

