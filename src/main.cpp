#include "../include/vm.h"
#include <fstream>
#include <iostream>
#include <sstream>

void repl()
{
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

std::string readFile(const char* path)
{
    std::ifstream fileStream(path);
    if (!fileStream) {
        std::cerr << "无法打开文件: " << path << std::endl;
        exit(1);
    }
    std::stringstream stringStream;
    stringStream << fileStream.rdbuf();
    return stringStream.str();
}

void runFile(const char* path)
{
    std::string source = readFile(path);
    int i = 0;
    InterpretResult result = interpret(source);

    if (result == INTERPRET_COMPILE_ERROR) {
        exit(65);
    }
    if (result == INTERPRET_RUNTIME_ERROR) {
        exit(70);
    }
}

int main(int argc, char* argv[])
{

    if (argc == 1) {
        repl();
    } else if (argc == 2) {
        runFile(argv[1]);
    } else {
        std::cerr << "Usage: cpplox [path]" << std::endl;
        exit(64);
    }

    return 0;
}
