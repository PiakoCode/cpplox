//
// Created by Piako on 23-12-9.
//

#ifndef CPPLOX_COMPILER_H
#define CPPLOX_COMPILER_H

#include "chunk.h"
#include "vm.h"
#include <memory>

bool compile(std::string& source, std::shared_ptr<Chunk> chunk);

#endif // CPPLOX_COMPILER_H
