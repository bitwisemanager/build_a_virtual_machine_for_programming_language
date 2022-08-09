#include "eva_vm.h"
#include "logger.h"

#include <iostream>

/**
 * Eva VM main executable
 */
int main(int argc, char const **argv) {

  EvaVM vm;

  auto result = vm.exec(R"(

    (set x (+ x 10))

    x

    (begin
        (var x 100)
        x)

    x

  )");

  std::cout << '\n';
  log(result);

  std::cout << "All done!\n";

  return 0;
}
