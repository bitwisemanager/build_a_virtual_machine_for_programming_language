/**
 * EVA Virtual Machine
 */

#ifndef EVA_VM__H
#define EVA_VM__H

#include <array>
#include <string>
#include <vector>

#include "eva_value.h"
#include "logger.h"
#include "op_code.h"

/**
 * Reads the current byte in the bytecode
 * and advances the ip pointer
 */
#define READ_BYTE() *ip++

/**
 * Gets a constant from the pool
 */
#define GET_CONST() (constants[READ_BYTE()])

/**
 * Binary operation
 */
#define BINARY_OP(op)                                                          \
  do {                                                                         \
    auto op2 = AS_NUMBER(pop());                                               \
    auto op1 = AS_NUMBER(pop());                                               \
    push(NUMBER(op1 op op2));                                                  \
  } while (0)

/**
 * Stack top (stack overflow after exceeding)
 */
#define STACK_LIMIT 512

class EvaVM {
public:
  EvaVM() {}

  /**
   * Pushes a value onto the stack
   */
  void push(const EvaValue &value) {
    if ((size_t)(sp - stack.begin()) == STACK_LIMIT)
      DIE << "push(): Stack overflow.\n";
    *sp = value;
    sp++;
  }

  /**
   *  Pops a value from the stack
   */
  EvaValue pop() {
    if (sp == stack.begin())
      DIE << "pop(): empty stack.\n";
    --sp;
    return *sp;
  }

  /**
   * Executes a program
   */
  EvaValue exec(const std::string &program) {
    // 1. Parse the program
    // auto ast = parser->parse(program);

    // 2. Compile program to Eva bytecode
    // code = compiler->compile(ast);

    constants.push_back(NUMBER(10));
    constants.push_back(NUMBER(3));
    constants.push_back(NUMBER(10));

    // (- (* 10 3) 10)
    code = {OP_CONST, 0, OP_CONST, 1, OP_MUL, OP_CONST, 2, OP_SUB, OP_HALT};

    // Set instruction pointer to the beginning:
    ip = &code[0];

    // Init the stack
    sp = &stack[0];

    return eval();
  }

  /**
   * Main eval lopp
   */
  EvaValue eval() {
    for (;;) {
      auto opcode = READ_BYTE();

      switch (opcode) {
      case OP_HALT:
        return pop();

        // ---------------
        // Constants:
      case OP_CONST:
        push(GET_CONST());
        break;

        // ---------------
        // Math ops:
      case OP_ADD:
        BINARY_OP(+);
        break;

      case OP_SUB:
        BINARY_OP(-);
        break;

      case OP_MUL:
        BINARY_OP(*);
        break;

      case OP_DIV:
        BINARY_OP(/);
        break;

      default:
        DIE << "Unknown opcode: " << std::hex << (uint64_t)opcode;
      }
    }
  }

public:
  /**
   * Instruction pointer (aka Program counter)
   */
  uint8_t *ip;

  /**
   * Stack pointer
   */
  EvaValue *sp;

  /**
   * Operands stack
   */
  std::array<EvaValue, STACK_LIMIT> stack;

  /**
   * Constant pool
   */
  std::vector<EvaValue> constants;

  /**
   * Bytecode
   */
  std::vector<uint8_t> code;
};

#endif
