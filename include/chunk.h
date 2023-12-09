//
// Created by Piako on 23-12-7.
//

#ifndef CPPLOX_CHUNK_H
#define CPPLOX_CHUNK_H

#include "common.h"
#include "value.h"

enum op {
    OP_CONSTANT,
    OP_ADD,
    OP_SUBTRACT,
    OP_MULTIPLY,
    OP_DIVIDE,
    OP_NEGATE,
    OP_RETURN,
};


class Chunk {
public:
    std::vector<uint8_t> code;
    std::vector<int> lines;
    std::vector<Value> constants;


    Chunk() = default;

    void writeChunk(uint8_t op, int line);

    size_t addConstant(double value);
    void debug(const char* name);
    size_t count() const;
};

#endif //CPPLOX_CHUNK_H
