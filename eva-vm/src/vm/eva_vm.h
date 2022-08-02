/**
 * EVA Virtual Machine
 */

#ifndef EVA_VM__H
#define EVA_VM__H

#include <string>
#include <vector>

#include "op_code.h"

/**
 * Reads the current byte in the bytecode
 * and advances the ip pointer
 */
#define READ_BYTE() *ip++

class EvaVM {
public:
  EvaVM() {}

  /**
   * Executes a program
   */
  void exec(const std::string &program) {
    // 1. Parse the program
    // auto ast = parser->parse(program);

    // 2. Compile program to Eva bytecode
    // code = compiler->compile(ast);

    code = {OP_HALT};

    // Set instruction pointer to the beginning:
    ip = &code[0];

    return eval();
  }

  /**
   * Main eval lopp
   */
  void eval() {
    for (;;) {
      switch (READ_BYTE()) {
      case OP_HALT:
        return;
      }
    }
  }

public:
  /**
   * Instruction pointer (aka Program counter)
   */
  uint8_t *ip;

  /**
   * Bytecode
   */
  std::vector<uint8_t> code;
};

#endif
