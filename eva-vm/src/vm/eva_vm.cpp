#include "eva_vm.h"
#include "logger.h"

#include <iostream>

/**
 * Eva VM main executable
 */
int main(int argc, char const **argv) {

  EvaVM vm;

  auto result = vm.exec(R"(

    (if (>= 5 10) 1 2)

  )");

  log(result);

  std::cout << "All done!\n";

  return 0;
}
