//
// Created by Piako on 23-12-7.
//

#include "chunk.h"


void Chunk::writeChunk(uint8_t byte, int line)
{
    this->code.emplace_back(byte);
    this->lines.emplace_back(line);
}

size_t Chunk::addConstant(Value value)
{
    this->constants.emplace_back(value);
    return this->constants.size() - 1;
}

void Chunk::debug(const char* name)
{
    std::printf("debug\n");
}
