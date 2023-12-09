//
// Created by Piako on 23-12-7.
//

#ifndef CPPLOX_DEBUG_H
#define CPPLOX_DEBUG_H

#include "common.h"
#include "chunk.h"

void disassembleChunk(const Chunk& chunk,const char* name);
int disassembleInstruction(const Chunk& chunk, int offset);

#endif //CPPLOX_DEBUG_H
