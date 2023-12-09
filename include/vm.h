//
// Created by Piako on 23-12-8.
//

#ifndef CPPLOX_VM_H
#define CPPLOX_VM_H

#include "chunk.h"

#define STACK_MAX 256;

class VM {
public:
    Chunk chunk;
    int ip = 0;

    std::stack<Value> value_stack;

    uint8_t readByte();

    VM() = default;

    ~VM() = default;

    Value readConstant();

    static void binaryOP(op OP);
};
extern VM vm;
enum InterpretResult {
    INTERPRET_OK,
    INTERPRET_COMPILE_ERROR,
    INTERPRET_RUNTIME_ERROR
};

InterpretResult interpret(Chunk &chunk);
InterpretResult interpret(const std::string& source);

InterpretResult run();

#endif //CPPLOX_VM_H
