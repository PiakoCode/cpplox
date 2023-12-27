#ifndef CPPLOX_STACK_H
#define CPPLOX_STACK_H

#include "common.h"
#include "value.h"
#include <cstddef>
#include <functional>

template <typename T>
class Stack {
private:
    std::vector<T> data;

public:
    void push(const T& value)
    {
        data.emplace_back(value);
    }

    void pop()
    {
        if (!data.empty()) {
            data.pop_back();
        }
    }

    const T& top()
    {
        return data[data.size() - 1];
    }

    size_t size()
    {
        return data.size();
    }

    bool empty()
    {
        return data.empty();
    }

    void clear()
    {
        data.clear();
    }

    // 遍历栈
    void range(const std::function<void(Value&)>& func)
    {
        for (auto& value : data) {
            func(value);
        }
    }

    // 倒数第distance + 1 个
    T& peek(uint8_t distance)
    {
        return data[data.size() - 1 - distance];
    }
};

#endif