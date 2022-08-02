#include "eva_vm.h"

#include <iostream>

/**
 * Eva VM main executable
 */
int main(int argc, char const **argv) {

  EvaVM vm;

  vm.exec(R"(
    42
  )");

  std::cout << "All done!\n";

  return 0;
}
