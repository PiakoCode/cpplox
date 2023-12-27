//
// Created by Piako on 23-12-7.
//

#ifndef CPPLOX_DEBUG_H
#define CPPLOX_DEBUG_H

#include "chunk.h"
#include "common.h"

void disassembleChunk(std::shared_ptr<Chunk> chunk, const char* name);
int disassembleInstruction(std::shared_ptr<Chunk> chunk, int offset);

#endif // CPPLOX_DEBUG_H
