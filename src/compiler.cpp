//
// Created by Piako on 23-12-9.
//
#include <format>
#include <iostream>
#include "../include/compiler.h"
#include "../include/scanner.h"


void compile(const std::string &source) {
    scanner = Scanner(source);
    //scanner = Scanner(source);

    int line = -1;
    while(true) {
        Token token = scanToken();
        if (token.line != line) {
            std::printf("%4d ",token.line);
            line = token.line;
        } else {
            std::printf("   | ");
        }

        std::printf("%2d '%s'\n", token.type, token.word.c_str());

        if (token.type == TOKEN_EOF)
            break;
    }

}
