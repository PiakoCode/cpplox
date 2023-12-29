//
// Created by Piako on 23-12-9.
//

#include "../include/compiler.h"
#include "../include/scanner.h"

#ifdef DEBUG_PRINT_CODE
#include "../include/debug.h"
#endif

class Parser {
public:
    Token current;
    Token previous;
    bool hadError { false };
    bool panicMode { false };
} parser;

enum Precedence { // 按优先级排列
    PREC_NONE,
    PREC_ASSIGNMENT, // =
    PREC_OR, // or
    PREC_AND, // and
    PREC_EQUALITY, // == !=
    PREC_COMPARISON, // < > <= >=
    PREC_TERM, // + -
    PREC_FACTOR, // * /
    PREC_UNARY, // ! -
    PREC_CALL, // . ()
    PREC_PRIMARY
};

typedef void (*ParseFn)();

class ParseRule {
public:
    ParseFn prefix {};
    ParseFn infix {};
    Precedence precedence {};
    ParseRule() = default;
    ParseRule(ParseFn func1, ParseFn func2, Precedence precedence)
    {
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
static void literal();
static void string();

ParseRule rules[40] = {
    rules[TOKEN_LEFT_PAREN] = { grouping, nullptr, PREC_NONE },
    rules[TOKEN_RIGHT_PAREN] = { nullptr, nullptr, PREC_NONE },
    rules[TOKEN_LEFT_BRACE] = { nullptr, nullptr, PREC_NONE },
    rules[TOKEN_RIGHT_BRACE] = { nullptr, nullptr, PREC_NONE },
    rules[TOKEN_COMMA] = { nullptr, nullptr, PREC_NONE },
    rules[TOKEN_DOT] = { nullptr, nullptr, PREC_NONE },
    rules[TOKEN_MINUS] = { unary, binary, PREC_TERM },
    rules[TOKEN_PLUS] = { nullptr, binary, PREC_TERM },
    rules[TOKEN_SEMICOLON] = { nullptr, nullptr, PREC_NONE },
    rules[TOKEN_SLASH] = { nullptr, binary, PREC_FACTOR },
    rules[TOKEN_STAR] = { nullptr, binary, PREC_FACTOR },
    rules[TOKEN_BANG] = { unary, nullptr, PREC_NONE },
    rules[TOKEN_BANG_EQUAL] = { nullptr, binary, PREC_EQUALITY },
    rules[TOKEN_EQUAL] = { nullptr, nullptr, PREC_NONE },
    rules[TOKEN_EQUAL_EQUAL] = { nullptr, binary, PREC_EQUALITY },
    rules[TOKEN_GREATER] = { nullptr, binary, PREC_COMPARISON },
    rules[TOKEN_GREATER_EQUAL] = { nullptr, binary, PREC_COMPARISON },
    rules[TOKEN_LESS] = { nullptr, binary, PREC_COMPARISON },
    rules[TOKEN_LESS_EQUAL] = { nullptr, binary, PREC_COMPARISON },
    rules[TOKEN_IDENTIFIER] = { nullptr, nullptr, PREC_NONE },
    rules[TOKEN_STRING] = { string, nullptr, PREC_NONE },
    rules[TOKEN_NUMBER] = { number, nullptr, PREC_NONE },
    rules[TOKEN_AND] = { nullptr, nullptr, PREC_NONE },
    rules[TOKEN_CLASS] = { nullptr, nullptr, PREC_NONE },
    rules[TOKEN_ELSE] = { nullptr, nullptr, PREC_NONE },
    rules[TOKEN_FALSE] = { literal, nullptr, PREC_NONE },
    rules[TOKEN_FOR] = { nullptr, nullptr, PREC_NONE },
    rules[TOKEN_FUN] = { nullptr, nullptr, PREC_NONE },
    rules[TOKEN_IF] = { nullptr, nullptr, PREC_NONE },
    rules[TOKEN_NIL] = { literal, nullptr, PREC_NONE },
    rules[TOKEN_OR] = { nullptr, nullptr, PREC_NONE },
    rules[TOKEN_PRINT] = { nullptr, nullptr, PREC_NONE },
    rules[TOKEN_RETURN] = { nullptr, nullptr, PREC_NONE },
    rules[TOKEN_SUPER] = { nullptr, nullptr, PREC_NONE },
    rules[TOKEN_THIS] = { nullptr, nullptr, PREC_NONE },
    rules[TOKEN_TRUE] = { literal, nullptr, PREC_NONE },
    rules[TOKEN_VAR] = { nullptr, nullptr, PREC_NONE },
    rules[TOKEN_WHILE] = { nullptr, nullptr, PREC_NONE },
    rules[TOKEN_ERROR] = { nullptr, nullptr, PREC_NONE },
    rules[TOKEN_EOF] = { nullptr, nullptr, PREC_NONE },
};

std::shared_ptr<Chunk> compilingChunk;

static ParseRule* getRule(TokenType type);
static void parsePrecedence(Precedence precedence);

static std::shared_ptr<Chunk> currentChunk()
{
    return compilingChunk;
}

static ParseRule* getRule(TokenType type)
{
    return &rules[type];
}

// ---------- Error handing----------
static void errorAt(Token& token, std::string& message)
{
    std::fprintf(stderr, "[line %d] Error", token.line);

    if (token.type == TOKEN_EOF) {
        std::fprintf(stderr, " at end");
    } else if (token.type == TOKEN_ERROR) {
        // Nothing
    } else {
        std::fprintf(stderr, " at %zu %s", token.word.length(), token.word.c_str());
    }
    std::fprintf(stderr, ": %s\n", message.c_str());
    parser.hadError = true;
}

static void error(std::string& message)
{
    errorAt(parser.previous, message);
}

static void errorAtCurrent(std::string& message)
{
    errorAt(parser.current, message);
}

// ---------- Error handing----------

static void advance()
{
    parser.previous = parser.current;

    while (true) {
        parser.current = scanToken();
        if (parser.current.type != TOKEN_ERROR) {
            break;
        }
        errorAtCurrent(parser.current.word);
    }
}

static void consume(TokenType type, std::string& message)
{
    if (parser.current.type == type) {
        advance();
        return;
    }

    errorAtCurrent(message);
}

// 发出字节码
static void emitByte(uint8_t byte)
{
    currentChunk()->writeChunk(byte, parser.previous.line);
}

static void emitBytes(uint8_t byte1, uint8_t byte2)
{
    emitByte(byte1);
    emitByte(byte2);
}

static void emitReturn()
{
    emitByte(OP_RETURN);
}

static uint8_t makeConstant(Value value)
{
    int constant = currentChunk()->addConstant(value);
    if (constant > UINT8_MAX) {
        std::string msg = "Too many constants in one chunk.";
        error(msg);
        return 0;
    }

    return (uint8_t)constant;
}

static void emitConstant(Value value)
{
    emitBytes(OP_CONSTANT, makeConstant(value));
}

static void endCompiler()
{
    emitReturn();

#ifdef DEBUG_PRINT_CODE
    if (!parser.hadError) {
        disassembleChunk(std::shared_ptr<Chunk>(currentChunk()), "code");
    }
#endif
}

static void parsePrecedence(Precedence precedence)
{
    advance();
    ParseFn prefixRule = getRule(parser.previous.type)->prefix;
    if (prefixRule == nullptr) {
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

static void number()
{
    double value = strtod(parser.previous.word.c_str(), nullptr);
    emitConstant(Value(value));
}

static void string()
{
    
    auto v = std::make_shared<obj::String>(parser.previous.word.c_str());
    vm.obj_list.emplace_back(v);
    emitConstant(Value(v.get()));
}

static void expression()
{
    parsePrecedence(PREC_ASSIGNMENT);
}

// 括号分组
static void grouping()
{
    expression();
    std::string msg = "Expect ')' after expression.";
    consume(TOKEN_RIGHT_PAREN, msg);
}

// 前缀表达式 -x !x
static void unary()
{
    auto operatorType = parser.previous.type;

    // Compile the operand
    parsePrecedence(PREC_UNARY);

    // Emit the operator instruction.
    switch (operatorType) {
    case TOKEN_MINUS:
        emitByte(OP_NEGATE);
        break;
    case TOKEN_BANG:
        emitByte(OP_NOT);
        break;
    default:
        return; // Unreachable;
    }
}

// 中缀表达式
static void binary()
{
    auto operatorType = parser.previous.type;
    auto rule = getRule(operatorType);
    parsePrecedence((Precedence)(rule->precedence + 1));

    switch (operatorType) {
    case TOKEN_BANG_EQUAL:
        emitBytes(OP_EQUAL, OP_NOT);
        break;
    case TOKEN_EQUAL_EQUAL:
        emitByte(OP_EQUAL);
        break;
    case TOKEN_GREATER:
        emitByte(OP_GREATER);
        break;
    case TOKEN_GREATER_EQUAL:
        emitBytes(OP_LESS, OP_NOT);
        break;
    case TOKEN_LESS:
        emitByte(OP_LESS);
        break;
    case TOKEN_LESS_EQUAL:
        emitBytes(OP_GREATER, OP_NOT);
        break;
    case TOKEN_PLUS:
        emitByte(OP_ADD);
        break;
    case TOKEN_MINUS:
        emitByte(OP_SUBTRACT);
        break;
    case TOKEN_STAR:
        emitByte(OP_MULTIPLY);
        break;
    case TOKEN_SLASH:
        emitByte(OP_DIVIDE);
        break;

    default:
        return; // Unreachable.
    }
}

static void literal()
{
    switch (parser.previous.type) {
    case TOKEN_FALSE:
        emitByte(OP_FALSE);
        break;
    case TOKEN_NIL:
        emitByte(OP_NIL);
        break;
    case TOKEN_TRUE:
        emitByte(OP_TRUE);
        break;
    default:
        return; // Unreachable
    }
}

bool compile(std::string& source, std::shared_ptr<Chunk> chunk)
{
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
