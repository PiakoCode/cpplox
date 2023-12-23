//
// Created by Piako on 23-12-7.
//


#include "../include/debug.h"

void disassembleChunk(std::shared_ptr<Chunk> chunk, const char *name) {
    std::printf("== %s == \n", name);

    int offset = 0;
    while (offset < chunk->code.size()) {
        offset = disassembleInstruction(chunk, offset);
    }
}

// Instruction
int simpleInstruction(const char *name, int offset) {
    printf("%s\n", name);
    return offset + 1;
}

// Instruction + constant
int constantInstruction(const char *name, std::shared_ptr<Chunk> chunk, int offset) {
    auto constant = chunk->code[offset + 1];
    std::printf("%-16s %4d '", name, constant);

    printValue(chunk->constants[constant]);

    printf("'\n");
    return offset + 2;
}


int disassembleInstruction(std::shared_ptr<Chunk> chunk, int offset) {
    std::printf("%04d ", offset);

    if (offset > 0 && chunk->lines[offset] == chunk->lines[offset - 1]) {
        std::printf("   | ");
    } else {
        std::printf("%4d ", chunk->lines[offset]);
    }


    uint8_t instruction = chunk->code[offset];
    switch (instruction) {
        case OP_RETURN:
            return simpleInstruction("OP_RETURN", offset);
        case OP_ADD:
            return simpleInstruction("OP_ADD",offset);
        case OP_SUBTRACT:
            return simpleInstruction("OP_SUBTRACT",offset);
        case OP_MULTIPLY:
            return simpleInstruction("OP_MULTIPLY",offset);
        case OP_DIVIDE:
            return simpleInstruction("OP_DIVIDE", offset);
        case OP_NEGATE:
            return simpleInstruction("OP_NEGATE", offset);
        case OP_CONSTANT:
            return constantInstruction("OP_CONSTANT", chunk, offset);
        default:
            std::printf("Unknown opcode %d\n", instruction);
            return offset + 1;
    }
}
