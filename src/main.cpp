#include <format>
#include <iostream>
#include "../include/common.h"
#include "../include/chunk.h"
#include "../include/debug.h"
#include "../include/vm.h"

int main(int argc, char *argv[]) {

    auto& chunk = vm.chunk;

    auto constant = chunk.addConstant(1.2);

    chunk.writeChunk(op::OP_CONSTANT,123);
    chunk.writeChunk(constant,123);

    constant = chunk.addConstant(3.4);
    chunk.writeChunk(OP_CONSTANT,123);
    chunk.writeChunk(constant,123);
    chunk.writeChunk(OP_ADD,123);

    constant = chunk.addConstant(5.6);
    chunk.writeChunk(OP_CONSTANT,123);
    chunk.writeChunk(constant,123);

    chunk.writeChunk(OP_DIVIDE,123);

    chunk.writeChunk(op::OP_NEGATE,123);
    chunk.writeChunk(op::OP_RETURN,123);
    chunk.debug("test chunk");
    disassembleChunk(chunk, "test chunk");
    interpret(chunk);

    return 0;
}

