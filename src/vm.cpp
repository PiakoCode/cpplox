//
// Created by Piako on 23-12-8.
//

#include "vm.h"
#include "compiler.h"
#include "debug.h"
#include "value.h"
VM vm;

static void runtimeError(const char* format, ...)
{
    std::va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
    fputs("\n", stderr);

    size_t instruction = vm.ip - 1;
    int line = vm.chunk->lines[instruction];
    fprintf(stderr, "[line %d] in script\n", line);
    vm.value_stack.clear();
}

static bool isFalsey(Value value)
{
    return Value::isNil(value) || (Value::isBool(value) && !value.asBool());
}

InterpretResult run()
{
    while (true) {
#ifdef DEBUG_TRACE_EXECUTION

        // 遍历栈
        std::printf("          ");
        if (!vm.value_stack.empty()) {
            vm.value_stack.range([](Value& v) {
                std::printf("[ ");
                printValue(v);
                std::printf(" ]");
            });
        }
        std::printf("\n");

        disassembleInstruction(vm.chunk, vm.ip);

#endif
        uint8_t instruction = vm.readByte();
        switch (instruction) {
        case OP_CONSTANT: {
            auto con = vm.readConstant();
            vm.value_stack.push(con);
            break;
        }

        case OP_NEGATE: {

            auto value = vm.value_stack.top();
            if (!Value::isNumber(value)) {
                runtimeError("Operand must be a number.");
                return INTERPRET_RUNTIME_ERROR;
            }
            vm.value_stack.pop();
            vm.value_stack.push(Value(-value.asNumber()));
            break;
        }
        case OP_NOT: {
            auto value = vm.value_stack.top();

            vm.value_stack.pop();
            vm.value_stack.push(Value(isFalsey(value)));
            break;
        }

        case OP_NIL:
            vm.value_stack.push(Value(nullptr));
            break;
        case OP_TRUE:
            vm.value_stack.push(Value(true));
            break;
        case OP_FALSE:
            vm.value_stack.push(Value(false));
            break;

        case OP_EQUAL: {
            auto value1 = vm.value_stack.top();
            vm.value_stack.pop();
            auto value2 = vm.value_stack.top();
            vm.value_stack.pop();

            vm.value_stack.push(Value(Value::valuesEqual(value1, value2)));
            break;
        }

        case OP_GREATER:
        case OP_LESS:

        case OP_ADD: {
            auto b = vm.value_stack.top();
            vm.value_stack.pop();
            auto a = vm.value_stack.top();
            vm.value_stack.pop();
            if (Value::isString(b) && Value::isString(a)) {
                auto& str1 = *a.asString();
                auto& str2 = *b.asString();

                auto* p = new obj::String(str1 + str2);
                vm.obj_list.emplace_back(p);
                vm.value_stack.push(Value((Obj*)(p)));
            } else if (Value::isNumber(a) && Value::isNumber(b)) {
                vm.value_stack.push(Value(a.asNumber() + b.asNumber()));
            } else {
                runtimeError(
                    "Operands must be two numbers or two strings.");
                return INTERPRET_RUNTIME_ERROR;
            }
            break;
        }
        case OP_SUBTRACT:
        case OP_MULTIPLY:
        case OP_DIVIDE: {
            if (VM::binaryOP(op(instruction)) == INTERPRET_RUNTIME_ERROR) {
                return INTERPRET_RUNTIME_ERROR;
            }
            break;
        }

        case OP_RETURN: {
            std::printf("-----\nOP_RETURN Value: ");
            printValue(vm.value_stack.top());
            std::printf("\n");
            return InterpretResult::INTERPRET_OK;
        }
        }
    }
}

InterpretResult interpret(std::shared_ptr<Chunk>& chunk)
{
    vm.chunk = chunk;
    vm.ip = 0;
    return run();
}

InterpretResult interpret(std::string& source)
{
    std::shared_ptr<Chunk> chunk = std::make_shared<Chunk>();

    if (!compile(source, chunk)) {
        return INTERPRET_COMPILE_ERROR;
    }

    vm.chunk = chunk;
    vm.ip = 0;

    auto result = run();

    return result;
}

InterpretResult VM::binaryOP(op OP)
{
    auto value2 = vm.value_stack.top();
    vm.value_stack.pop();
    auto value1 = vm.value_stack.top();
    vm.value_stack.pop();

    if (!Value::isNumber(value1) || !Value::isNumber(value2)) {
        runtimeError("Operands must be numbers.");
        return INTERPRET_RUNTIME_ERROR;
    }
    auto a = value1.data.number;
    auto b = value2.data.number;
    double c = 0;
    switch (OP) {
    case OP_GREATER:
        vm.value_stack.push(Value(a > b));
        return INTERPRET_OK;

    case OP_LESS:
        vm.value_stack.push(Value(a < b));
        return INTERPRET_OK;

    case OP_ADD:
        c = a + b;
        break;
    case OP_SUBTRACT:
        c = a - b;
        break;
    case OP_MULTIPLY:
        c = a * b;
        break;
    case OP_DIVIDE:
        c = a / b;
        break;
    default:
        runtimeError("Unknown error!");
        return INTERPRET_RUNTIME_ERROR;
    }
    vm.value_stack.push(Value(c));
    return INTERPRET_OK;
}

uint8_t VM::readByte()
{
    return vm.chunk->code[ip++];
}

Value VM::readConstant()
{
    return vm.chunk->constants[readByte()];
}
