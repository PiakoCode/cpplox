//
// Created by Piako on 23-12-8.
//


#include "../include/vm.h"
#include "../include/debug.h"
#include "../include/compiler.h"

VM vm;

InterpretResult run() {
    while (true) {
#ifdef DEBUG_TRACE_EXECUTION

        // 遍历栈(从栈底开始)
        std::printf("          ");
        std::stack<Value> tmp_stack;
        while (!vm.value_stack.empty()) {
            tmp_stack.push(vm.value_stack.top());
            vm.value_stack.pop();
        }
        while (!tmp_stack.empty()) {
            std::printf("[ ");
            printValue(tmp_stack.top());
            vm.value_stack.push(tmp_stack.top());
            tmp_stack.pop();
            std::printf(" ]");
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
            case OP_ADD:
            case OP_SUBTRACT:
            case OP_MULTIPLY:
            case OP_DIVIDE: {
                VM::binaryOP(op(instruction));
                break;
            }
            case OP_NEGATE: {
                auto value = vm.value_stack.top();
                vm.value_stack.pop();
                vm.value_stack.push(-value);
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

InterpretResult interpret(Chunk &chunk) {
    vm.chunk = chunk;
    vm.ip = 0;
    return run();
}

InterpretResult interpret(const std::string& source) {
    compile(source);
    return INTERPRET_OK;
}

void VM::binaryOP(op OP) {
    auto a = vm.value_stack.top();
    vm.value_stack.pop();
    auto b = vm.value_stack.top();
    vm.value_stack.pop();
    switch (OP) {
        case OP_ADD:
            vm.value_stack.push(a + b);
            break;
        case OP_SUBTRACT:
            vm.value_stack.push(a - b);
            break;
        case OP_MULTIPLY:
            vm.value_stack.push(a * b);
            break;
        case OP_DIVIDE:
            vm.value_stack.push(a / b);
            break;
    }
}


uint8_t VM::readByte() {
    return vm.chunk.code[ip++];
}

Value VM::readConstant() {
    return vm.chunk.constants[readByte()];
}
