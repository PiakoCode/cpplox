//
// Created by Piako on 23-12-9.
//

#include "../include/compiler.h"
#include "../include/scanner.h"
#include <memory>

#ifdef DEBUG_PRINT_CODE
#include "../include/debug.h"
#endif

class Parser {
public:
    Token current;
    Token previous;
    bool hadError{false};
    bool panicMode{false};
} parser;

enum Precedence { // 按优先级排列
  PREC_NONE,
  PREC_ASSIGNMENT,  // =
  PREC_OR,          // or
  PREC_AND,         // and
  PREC_EQUALITY,    // == !=
  PREC_COMPARISON,  // < > <= >=
  PREC_TERM,        // + -
  PREC_FACTOR,      // * /
  PREC_UNARY,       // ! -
  PREC_CALL,        // . ()
  PREC_PRIMARY
};

typedef void (*ParseFn)();

class ParseRule {
    public:
    ParseFn prefix{};
    ParseFn infix{};
    Precedence precedence{};
    ParseRule() = default;
    ParseRule(ParseFn func1, ParseFn func2, Precedence precedence) {
        this->prefix = func1;
        this->infix = func2;
        this->precedence = precedence;
    }
};

static void expression();
static void grouping();
static void unary();
static void binary();
static void number();

ParseRule rules[40]= {
rules[TOKEN_LEFT_PAREN]    = {grouping, NULL,   PREC_NONE},
rules[TOKEN_RIGHT_PAREN]   = {NULL,     NULL,   PREC_NONE},
rules[TOKEN_LEFT_BRACE]    = {NULL,     NULL,   PREC_NONE},
rules[TOKEN_RIGHT_BRACE]   = {NULL,     NULL,   PREC_NONE},
rules[TOKEN_COMMA]         = {NULL,     NULL,   PREC_NONE},
rules[TOKEN_DOT]           = {NULL,     NULL,   PREC_NONE},
rules[TOKEN_MINUS]         = {unary,    binary, PREC_TERM},
rules[TOKEN_PLUS]          = {NULL,     binary, PREC_TERM},
rules[TOKEN_SEMICOLON]     = {NULL,     NULL,   PREC_NONE},
rules[TOKEN_SLASH]         = {NULL,     binary, PREC_FACTOR},
rules[TOKEN_STAR]          = {NULL,     binary, PREC_FACTOR},
rules[TOKEN_BANG]          = {NULL,     NULL,   PREC_NONE},
rules[TOKEN_BANG_EQUAL]    = {NULL,     NULL,   PREC_NONE},
rules[TOKEN_EQUAL]         = {NULL,     NULL,   PREC_NONE},
rules[TOKEN_EQUAL_EQUAL]   = {NULL,     NULL,   PREC_NONE},
rules[TOKEN_GREATER]       = {NULL,     NULL,   PREC_NONE},
rules[TOKEN_GREATER_EQUAL] = {NULL,     NULL,   PREC_NONE},
rules[TOKEN_LESS]          = {NULL,     NULL,   PREC_NONE},
rules[TOKEN_LESS_EQUAL]    = {NULL,     NULL,   PREC_NONE},
rules[TOKEN_IDENTIFIER]    = {NULL,     NULL,   PREC_NONE},
rules[TOKEN_STRING]        = {NULL,     NULL,   PREC_NONE},
rules[TOKEN_NUMBER]        = {number,   NULL,   PREC_NONE},
rules[TOKEN_AND]           = {NULL,     NULL,   PREC_NONE},
rules[TOKEN_CLASS]         = {NULL,     NULL,   PREC_NONE},
rules[TOKEN_ELSE]          = {NULL,     NULL,   PREC_NONE},
rules[TOKEN_FALSE]         = {NULL,     NULL,   PREC_NONE},
rules[TOKEN_FOR]           = {NULL,     NULL,   PREC_NONE},
rules[TOKEN_FUN]           = {NULL,     NULL,   PREC_NONE},
rules[TOKEN_IF]            = {NULL,     NULL,   PREC_NONE},
rules[TOKEN_NIL]           = {NULL,     NULL,   PREC_NONE},
rules[TOKEN_OR]            = {NULL,     NULL,   PREC_NONE},
rules[TOKEN_PRINT]         = {NULL,     NULL,   PREC_NONE},
rules[TOKEN_RETURN]        = {NULL,     NULL,   PREC_NONE},
rules[TOKEN_SUPER]         = {NULL,     NULL,   PREC_NONE},
rules[TOKEN_THIS]          = {NULL,     NULL,   PREC_NONE},
rules[TOKEN_TRUE]          = {NULL,     NULL,   PREC_NONE},
rules[TOKEN_VAR]           = {NULL,     NULL,   PREC_NONE},
rules[TOKEN_WHILE]         = {NULL,     NULL,   PREC_NONE},
rules[TOKEN_ERROR]         = {NULL,     NULL,   PREC_NONE},
rules[TOKEN_EOF]           = {NULL,     NULL,   PREC_NONE},
};

std::shared_ptr<Chunk> compilingChunk;


static ParseRule* getRule(TokenType type);
static void parsePrecedence(Precedence precedence);


static std::shared_ptr<Chunk> currentChunk() {
    return compilingChunk;
}


static ParseRule* getRule(TokenType type) {
    return &rules[type];
}


// ---------- Error handing----------
static void errorAt(Token& token, std::string& message) {
    std::fprintf(stderr, "[line %d] Error", token.line);

    if (token.type == TOKEN_EOF) {
        std::fprintf(stderr, " at end");
    } else if (token.type == TOKEN_ERROR) {
        // Nothing
    }  else {
        std::fprintf(stderr, " at %zu %s", token.word.length(), token.word.c_str());
    }
    std::fprintf(stderr, ": %s\n", message.c_str());
    parser.hadError = true;
}

static void error(std::string& message) {
    errorAt(parser.previous, message);
}

static void errorAtCurrent(std::string& message) {
    errorAt(parser.current, message);
}

// ---------- Error handing----------

static void advance() {
    parser.previous = parser.current;

    while (true) {
        parser.current = scanToken();
        if (parser.current.type != TOKEN_ERROR)
            break;
        errorAtCurrent(parser.current.word);
    }
}

static void consume(TokenType type, std::string& message) {
    if (parser.current.type == type) {
        advance();
        return;
    }

    errorAtCurrent(message);
}

// 发出字节码
static void emitByte(uint8_t byte) {
    currentChunk()->writeChunk(byte, parser.previous.line);
}

static void emitBytes(uint8_t byte1, uint8_t byte2) {
    emitByte(byte1);
    emitByte(byte2);
}

static void emitReturn() {
    emitByte(OP_RETURN);
}

static uint8_t makeConstant(double value) {
    int constant = currentChunk()->addConstant(value);
    if (constant > UINT8_MAX) {
        std::string msg = "Too many constants in one chunk.";
        error(msg);
        return 0;
    }

    return (uint8_t)constant;
}

static void emitConstant(double value) {
    emitBytes(OP_CONSTANT,makeConstant(value));
}


static void endCompiler() {
    emitReturn();

    #ifdef DEBUG_PRINT_CODE
  if (!parser.hadError) {
    disassembleChunk(std::shared_ptr<Chunk>(currentChunk()), "code");
  }
#endif
}


static void parsePrecedence(Precedence precedence) {
    advance();
    ParseFn prefixRule = getRule(parser.previous.type)->prefix;
    if (prefixRule == NULL) {
        std::string msg = "Expect expression.";
        error(msg);
        return;
    }

    prefixRule();

    while (precedence <= getRule(parser.current.type)->precedence) {
        advance();
        ParseFn infixRule = getRule(parser.previous.type)->infix;
        infixRule();
    }
}



static void number() {
    double value = strtod(parser.previous.word.c_str(), NULL);
    emitConstant(value);
}



static void expression() {
    parsePrecedence(PREC_ASSIGNMENT);
}

// 括号分组
static void grouping() {
    expression();
    std::string msg = "Expect ')' after expression.";
    consume(TOKEN_RIGHT_PAREN, msg);
}

// 前缀表达式 -x
static void unary() {
    auto operatorType = parser.previous.type;

    // Compile the operand
    parsePrecedence(PREC_UNARY);

    // Emit the operator instruction.
    switch (operatorType) {
        case TOKEN_MINUS: emitByte(OP_NEGATE); break;
        default: return ; // Unreachable;
    }
}

// 中缀表达式
static void binary() {
    auto operatorType = parser.previous.type;
    auto rule = getRule(operatorType);
    parsePrecedence((Precedence)(rule->precedence  + 1));

    switch (operatorType) {
        case TOKEN_PLUS: emitByte(OP_ADD); break;
        case TOKEN_MINUS: emitByte(OP_SUBTRACT); break;
        case TOKEN_STAR: emitByte(OP_MULTIPLY); break;
        case TOKEN_SLASH: emitByte(OP_DIVIDE); break;
        default: return; // Unreachable.
    }
}


bool compile(std::string& source,std::shared_ptr<Chunk> chunk) {
    scanner = Scanner(source);

    compilingChunk = chunk;

    parser.hadError = false;
    parser.panicMode = false;

    advance(); // 启动!
    expression();

    std::string msg = "Expect end of expression.";
    consume(TOKEN_EOF, msg);

    endCompiler();
    return !parser.hadError;
}
