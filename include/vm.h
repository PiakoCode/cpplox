//
// Created by Piako on 23-12-8.
//

#ifndef CPPLOX_VM_H
#define CPPLOX_VM_H

#include "chunk.h"
#include "stack.h"

#define STACK_MAX 256;

enum InterpretResult {
    INTERPRET_OK,
    INTERPRET_COMPILE_ERROR,
    INTERPRET_RUNTIME_ERROR
};

class VM {
public:
    std::shared_ptr<Chunk> chunk { std::make_shared<Chunk>() };
    // The index of chunk.code
    int ip = 0;

    Stack<Value> value_stack;

    uint8_t readByte();

    VM() = default;

    ~VM() = default;

    Value readConstant();

    static InterpretResult binaryOP(op OP);

    VM operator=(VM&&) = delete;

    VM(VM&&) = delete;
};

extern VM vm;

InterpretResult interpret(std::shared_ptr<Chunk>& chunk);

InterpretResult interpret(std::string& source);

InterpretResult run();

#endif // CPPLOX_VM_H
