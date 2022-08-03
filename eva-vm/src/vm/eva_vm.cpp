#include "eva_vm.h"
#include "logger.h"

#include <iostream>

/**
 * Eva VM main executable
 */
int main(int argc, char const **argv) {

  EvaVM vm;

  auto result = vm.exec(R"(
    (+ "Hello, " "world!")
  )");

  log(AS_CPPSTRING(result));

  std::cout << "All done!\n";

  return 0;
}
