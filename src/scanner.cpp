//
// Created by Piako on 23-12-9.
//
#include "../include/scanner.h"


Scanner scanner;

Scanner::Scanner(const std::string &source) {
    this->words = source;
    this->start = 0;
    this->current = 0;
    this->line = 1;
}


bool isAtEnd() {
    return scanner.current == scanner.words.length();
}


Token makeToken(TokenType type) {
    Token token;
    token.type = type;
    token.word = scanner.words.substr(scanner.start, scanner.current - scanner.start);
    token.line = scanner.line;
    return token;

}

Token errorToken(const std::string &message) {
    Token token;
    token.type = TOKEN_ERROR;
    token.word = message;
    token.line = scanner.line;
    return token;
}

char advance() {
    return scanner.words[scanner.current++];
}

char peek() {
    return scanner.words[scanner.current];
}

char peekNext() {
    if (isAtEnd())
        return '\0';
    return scanner.words[scanner.current + 1];
}


bool match(char expected) {
    if (isAtEnd())
        return false;
    if (scanner.words[scanner.current] != expected)
        return false;
    scanner.current++;
    return true;
}


void skipWhitespace() {
    while (true) {
        char c = peek();
        if (c == ' ' || c == '\r' || c == '\t') {
            advance();
        } else if (c == '\n') {
            scanner.line++;
            advance();
            break;
        } else if (c == '/') {
            if (peekNext() == '/') {
                while (peek() != '\n' && !isAtEnd())
                    advance();
            } else {
                return;
            }
        } else {
            return;
        }

    }
}

Token string() {
    while (peek() != '"' && !isAtEnd()) {
        if (peek() == '\n')
            scanner.line++;
        advance();
    }

    if (isAtEnd())
        return errorToken("Unterminated string.");
    advance();
    return makeToken(TOKEN_STRING);
}


Token number() {
    while (isdigit(peek()))
        advance();
    if (peek() == '.' && isdigit(peekNext())) {
        advance();
        while (isdigit(peek()))
            advance();
    }

    return makeToken(TOKEN_NUMBER);
}

TokenType checkKeyword(int start, int length, const char *rest, TokenType type) {
    std::string_view keyword = scanner.words.substr(start, length);
    if (keyword == rest) {
        return type;
    }

    return TOKEN_IDENTIFIER;

}

TokenType identifierType() {
    switch (scanner.words[scanner.start]) {
        case 'a':
            return checkKeyword(1, 2, "nd", TOKEN_AND);
        case 'c':
            return checkKeyword(1, 4, "lass", TOKEN_CLASS);
        case 'e':
            return checkKeyword(1, 3, "lse", TOKEN_ELSE);
        case 'f':
            if (scanner.current - scanner.start > 1) {
                switch (scanner.words[scanner.start + 1]) {
                    case 'a':
                        return checkKeyword(2, 3, "lse", TOKEN_FALSE);
                    case 'o':
                        return checkKeyword(2, 1, "r", TOKEN_FOR);
                    case 'u':
                        return checkKeyword(2, 1, "n", TOKEN_FUN);
                }
            }
            break;
        case 'i':
            return checkKeyword(1, 1, "f", TOKEN_IF);
        case 'n':
            return checkKeyword(1, 2, "il", TOKEN_NIL);
        case 'o':
            return checkKeyword(1, 1, "r", TOKEN_OR);
        case 'p':
            return checkKeyword(1, 4, "rint", TOKEN_PRINT);
        case 'r':
            return checkKeyword(1, 5, "eturn", TOKEN_RETURN);
        case 's':
            return checkKeyword(1, 4, "uper", TOKEN_SUPER);
        case 't':
            if (scanner.current - scanner.start > 1) {
                switch (scanner.words[scanner.start + 1]) {
                    case 'h':
                        return checkKeyword(2, 2, "is", TOKEN_THIS);
                    case 'r':
                        return checkKeyword(2, 2, "ue", TOKEN_TRUE);
                }
            }
            break;
        case 'v':
            return checkKeyword(1, 2, "ar", TOKEN_VAR);
        case 'w':
            return checkKeyword(1, 4, "hile", TOKEN_WHILE);
    }
    return TOKEN_IDENTIFIER;
}

Token identifier() {
    while (isalpha(peek()) || peek() == '_' || isdigit(peek()))
        advance();
    return makeToken(identifierType());
}


Token scanToken() {
    skipWhitespace();
    scanner.start = scanner.current;

    if (isAtEnd()) {
        return makeToken(TOKEN_EOF);
    }

    char c = advance();

    if (std::isdigit(c))
        return number();
    if (isalpha(c) || c == '_')
        return identifier();

    switch (c) {
        case '(':
            return makeToken(TOKEN_LEFT_PAREN);
        case ')':
            return makeToken(TOKEN_RIGHT_PAREN);
        case '{':
            return makeToken(TOKEN_LEFT_BRACE);
        case '}':
            return makeToken(TOKEN_RIGHT_BRACE);
        case ';':
            return makeToken(TOKEN_SEMICOLON);
        case ',':
            return makeToken(TOKEN_COMMA);
        case '.':
            return makeToken(TOKEN_DOT);
        case '-':
            return makeToken(TOKEN_MINUS);
        case '+':
            return makeToken(TOKEN_PLUS);
        case '/':
            return makeToken(TOKEN_SLASH);
        case '*':
            return makeToken(TOKEN_STAR);
        case '!':
            return makeToken(
                    match('=') ? TOKEN_BANG_EQUAL : TOKEN_BANG);
        case '=':
            return makeToken(
                    match('=') ? TOKEN_EQUAL_EQUAL : TOKEN_EQUAL);
        case '<':
            return makeToken(
                    match('=') ? TOKEN_LESS_EQUAL : TOKEN_LESS);
        case '>':
            return makeToken(
                    match('=') ? TOKEN_GREATER_EQUAL : TOKEN_GREATER);
        case '"':
            return string();
    }

    return errorToken("Unexpected character.");

}