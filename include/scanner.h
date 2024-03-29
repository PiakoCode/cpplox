//
// Created by Piako on 23-12-9.
//

#ifndef CPPLOX_SCANNER_H
#define CPPLOX_SCANNER_H
#include "common.h"

enum TokenType {
    // Single-character tokens. 单字符词法
    TOKEN_LEFT_PAREN, TOKEN_RIGHT_PAREN,
    TOKEN_LEFT_BRACE, TOKEN_RIGHT_BRACE,
    TOKEN_COMMA, TOKEN_DOT, TOKEN_MINUS, TOKEN_PLUS,
    TOKEN_SEMICOLON, TOKEN_SLASH, TOKEN_STAR,
    // One or two character tokens. 一或两字符词法
    TOKEN_BANG, TOKEN_BANG_EQUAL,
    TOKEN_EQUAL, TOKEN_EQUAL_EQUAL,
    TOKEN_GREATER, TOKEN_GREATER_EQUAL,
    TOKEN_LESS, TOKEN_LESS_EQUAL,
    // Literals. 字面量
    TOKEN_IDENTIFIER, TOKEN_STRING, TOKEN_NUMBER,
    // Keywords. 关键字
    TOKEN_AND, TOKEN_CLASS, TOKEN_ELSE, TOKEN_FALSE,
    TOKEN_FOR, TOKEN_FUN, TOKEN_IF, TOKEN_NIL, TOKEN_OR,
    TOKEN_PRINT, TOKEN_RETURN, TOKEN_SUPER, TOKEN_THIS,
    TOKEN_TRUE, TOKEN_VAR, TOKEN_WHILE,

    TOKEN_ERROR, TOKEN_EOF
};


class Token {
public:
    TokenType type;
    std::string word; // start
    int line;
};


class Scanner {
public:
    std::string words;
    int start{};
    int current{};
    int line{};

    Scanner() = default;

    explicit Scanner(const std::string &source);
};

extern Scanner scanner;

bool isAtEnd();

Token scanToken();

Token makeToken(TokenType type);

Token errorToken(const std::string &message);


Token identifier();

/**
 * @brief 比较字符串
 * @param start 比较开始位置
 * @param length 长度
 * @param rest 剩余的字符串(需要比较的部分)
 * @param type 比较的类型
 * @return type or 字面量
 */
TokenType checkKeyword(int start, int length, const char *rest, TokenType type);

/**
 * @brief 判断 Token 是关键词还是字面量
 * @return Token 的类型
 */
TokenType identifierType();


#endif //CPPLOX_SCANNER_H
