/**
 * EVA Virtual Machine
 */

#ifndef EVA_VM__H
#define EVA_VM__H

#include <array>
#include <string>
#include <vector>

#include "../bytecode/op_code.h"
#include "../parser/eva_parser.h"
#include "eva_compiler.h"
#include "eva_value.h"
#include "logger.h"

using syntax::EvaParser;

/**
 * Reads the current byte in the bytecode
 * and advances the ip pointer
 */
#define READ_BYTE() *ip++

/**
 * Reads a short word (2 bytes)
 */
#define READ_SHORT() (ip += 2, (ip[-2] << 8) | ip[-1])

/**
 * Converts bytecode index to a pointer
 */
#define TO_ADDRESS(index) (&co->code[index])

/**
 * Gets a constant from the pool
 */
#define GET_CONST() (co->constants[READ_BYTE()])

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
 * Generic values comparison
 */
#define COMPARE_VALUES(op, v1, v2)                                             \
  do {                                                                         \
    bool res;                                                                  \
    switch (op) {                                                              \
    case 0:                                                                    \
      res = v1 < v2;                                                           \
      break;                                                                   \
    case 1:                                                                    \
      res = v1 > v2;                                                           \
      break;                                                                   \
    case 2:                                                                    \
      res = v1 == v2;                                                          \
      break;                                                                   \
    case 3:                                                                    \
      res = v1 >= v2;                                                          \
      break;                                                                   \
    case 4:                                                                    \
      res = v1 <= v2;                                                          \
      break;                                                                   \
    case 5:                                                                    \
      res = v1 != v2;                                                          \
      break;                                                                   \
    }                                                                          \
    push(BOOLEAN(res));                                                        \
  } while (0)

/**
 * Stack top (stack overflow after exceeding)
 */
#define STACK_LIMIT 512

class EvaVM {
public:
  EvaVM()
      : parser(std::make_unique<EvaParser>()),
        compiler(std::make_unique<EvaCompiler>()) {}

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
    auto ast = parser->parse(program);

    // 2. Compile program to Eva bytecode
    co = compiler->compile(ast);

    // Init the stack
    sp = &stack[0];

    // Set instruction pointer to the beginning:
    ip = &co->code[0];

    // Debug disassembly
    compiler->disassembleBytecode();

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
      case OP_ADD: {

        auto op2 = pop();
        auto op1 = pop();

        /// Numeric addition
        if (IS_NUMBER(op1) && IS_NUMBER(op2)) {
          auto v1 = AS_NUMBER(op1);
          auto v2 = AS_NUMBER(op2);
          push(NUMBER(v1 + v2));
        }

        // String concaternation
        if (IS_STRING(op1) && IS_STRING(op2)) {
          auto s1 = AS_CPPSTRING(op1);
          auto s2 = AS_CPPSTRING(op2);
          push(ALLOC_STRING(s1 + s2));
        }
      } break;

      case OP_SUB:
        BINARY_OP(-);
        break;

      case OP_MUL:
        BINARY_OP(*);
        break;

      case OP_DIV:
        BINARY_OP(/);
        break;

        // -----------------------
        // Comparison
      case OP_COMPARE: {
        auto op = READ_BYTE();
        auto op2 = pop();
        auto op1 = pop();

        if (IS_NUMBER(op1) && IS_NUMBER(op2)) {
          auto v1 = AS_NUMBER(op1);
          auto v2 = AS_NUMBER(op2);
          COMPARE_VALUES(op, v1, v2);
        } else if (IS_STRING(op1) && IS_STRING(op2)) {
          auto s1 = AS_STRING(op1);
          auto s2 = AS_STRING(op2);
          COMPARE_VALUES(op, s1, s2);
        }
      } break;

        // -----------------------
        // Conditional jump
      case OP_JMP_IF_ELSE: {
        auto cond = AS_BOOLEAN(pop());

        auto address = READ_SHORT();

        if (!cond) {
          ip = TO_ADDRESS(address);
        }
      } break;

        // -----------------------
        // Unconditional jump
      case OP_JMP: {
        ip = TO_ADDRESS(READ_SHORT());
      } break;

      default:
        DIE << "Unknown opcode: " << std::hex << (uint64_t)opcode;
      }
    }
  }

public:
  /**
   * Parser
   */
  std::unique_ptr<EvaParser> parser;

  /**
   * Compiler
   */
  std::unique_ptr<EvaCompiler> compiler;

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
   * Code object
   */
  CodeObject *co;
};

#endif
