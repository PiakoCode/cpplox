#include <iostream>
#include <fstream>
#include <sstream>
#include "../include/common.h"
#include "../include/chunk.h"
#include "../include/debug.h"
#include "../include/vm.h"


void repl() {
    std::string line;
    while (true) {
        std::cout << "> ";
        std::getline(std::cin, line);

        if (line == "quit") {
            break;
        }

        interpret(line);

    }

}

std::string readFile(const char *path) {
    std::ifstream fileStream(path);
    if (!fileStream) {
        std::cerr << "无法打开文件: " << path << std::endl;
        exit(1);
    }
    std::stringstream stringStream;
    stringStream << fileStream.rdbuf();
    return stringStream.str();
}

void runFile(const char *path) {
    std::string source = readFile(path);
    InterpretResult result = interpret(source);

    if (result == INTERPRET_COMPILE_ERROR)
        exit(65);
    if (result == INTERPRET_RUNTIME_ERROR)
        exit(70);
}


int main(int argc, char *argv[]) {

    if (argc == 1) {
        repl();
    } else if (argc == 2) {
        runFile(argv[1]);
    } else {
        std::cerr << "Usage: cpplox [path]" << std::endl;
        exit(64);
    }

    // auto& chunk = vm.chunk;
    // /**
    //  * 添加常数
    //  * 1. 向常数数组添加值
    //  * 2. 添加 OP_CONSTANT 操作，和 常数idx
    //  */
    // auto constant = chunk->addConstant(1.2);
    // chunk->writeChunk(OP_CONSTANT, 123);
    // chunk->writeChunk(constant, 123);

    // constant = chunk->addConstant(3.4);
    // chunk->writeChunk(OP_CONSTANT, 123);
    // chunk->writeChunk(constant, 123);


    // chunk->writeChunk(OP_ADD, 123);

    // constant = chunk->addConstant(5.6);
    // chunk->writeChunk(OP_CONSTANT, 123);
    // chunk->writeChunk(constant, 123);

    // chunk->writeChunk(OP_DIVIDE, 123);

    // chunk->writeChunk(OP_NEGATE, 123);
    // chunk->writeChunk(OP_RETURN, 123);
    // chunk->debug("test chunk");
    // disassembleChunk(chunk, "test chunk");
    // interpret(chunk);

    return 0;
}

