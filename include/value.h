//
// Created by Piako on 23-12-8.
//
#ifndef CPPLOX_VALUE_H
#define CPPLOX_VALUE_H

#include "common.h"
#include "object.h"
#include "object/String.h"
enum class ValueType {
    Bool,
    Nil,
    Number,
    Obj
};

class Value {
public:

    // TODO:改为使用std::variant
    union {
        bool boolean;
        double number;
        Obj* obj; // 字符串/对象...(指针)
    } data;
    explicit Value(bool b)
        : type(ValueType::Bool)
        , data { .boolean = b } {};
    explicit Value(std::nullptr_t)
        : type(ValueType::Nil)
        , data { .number = 0 } {};
    explicit Value(double number)
        : type(ValueType::Number)
        , data { .number = number } {};
    explicit Value(Obj* obj)
        : type(ValueType::Obj)
        , data { .obj = obj } {};
    Value() = delete;

    ValueType getType() const
    {
        return type;
    }

    bool asBool() const
    {
        return data.boolean;
    }

    double asNumber() const
    {
        return data.number;
    }
    Obj* asObj() const
    {
        return data.obj;
    }
    obj::String* asString() const
    {
        return (obj::String*)(data.obj);
    }

    char* asCString() const
    {
        return asString()->c_str();
    }

    static bool isBool(Value& value)
    {
        return value.type == ValueType::Bool;
    }
    static bool isNil(Value& value)
    {
        return value.type == ValueType::Nil;
    }
    static bool isNumber(Value& value)
    {
        return value.type == ValueType::Number;
    }
    static bool isObj(Value& value)
    {
        return value.type == ValueType::Obj;
    }
    static bool isObjType(Value value, ObjType type)
    {
        return Value::isObj(value) && value.asObj()->type == type;
    }
    static bool isString(Value& value)
    {

        return isObjType(value, ObjType::OBJ_STRING);
    }

    static bool valuesEqual(Value a, Value b)
    {
        // 类型不同
        if (a.type != b.type) {
            return false;
        }
        // 类型相同 比较值
        switch (a.type) {
        case ValueType::Bool:
            return a.asBool() == b.asBool();
        case ValueType::Nil:
            return true;
        case ValueType::Number:
            return a.asNumber() == b.asNumber();
        case ValueType::Obj: {
            obj::String a_str = *a.asString();
            obj::String b_str = *b.asString();
            if (a_str == b_str) {
                return true;
            }
        }
        default:
            return false;
        }
    }
private:
    ValueType type;
};

// typedef double Value;

void printValue(Value value);

void printObject(Value value);

#endif // CPPLOX_VALUE_H
