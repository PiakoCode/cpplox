//
// Created by Piako on 23-12-8.
//
#ifndef CPPLOX_VALUE_H
#define CPPLOX_VALUE_H

#include "common.h"

enum class ValueType{
    Bool,
    Nil,
    Number
};

class Value {
    public:
    ValueType type;

    // TODO:改为使用std::variant
    union {
        bool boolean;
        double number;
    } data;
    explicit Value(bool b):type(ValueType::Bool), data{.boolean = b}{}
    explicit Value(std::nullptr_t):type(ValueType::Nil), data{.number = 0}{}
    explicit Value(double number):type(ValueType::Number),data{.number = number} {}
    Value() = delete;

    ValueType getType() const {
        return type;
    }


    bool asBool()  const {
        return data.boolean;
    }

    double asNumber() const {
        return data.number;
    }

    static bool isBool(Value& value)  {
        return value.type == ValueType::Bool;
    }
    static bool isNil(Value& value) {
        return value.type == ValueType::Nil;
    }
    static bool isNumber(Value& value) {
        return value.type == ValueType::Number;
    }

    static bool valuesEqual(Value a, Value b) {
        if(a.type != b.type)
            return false;
        switch (a.type) {

        case ValueType::Bool:
            return a.asBool() == b.asBool();
        case ValueType::Nil:
            return true;
        case ValueType::Number:
            return a.asNumber() == b.asNumber();
        default:
            return false;
        }
    }

};

// typedef double Value;

void printValue(Value value);

#endif //CPPLOX_VALUE_H
