#include <iostream>
#include <string>

#include "nfa.hpp"
#include "parser.hpp"
// #include "regex.hpp"

int main() {
  std::string exp = "(a|b)*c";

  std::cout << Nfa(exp);

  return 0;
}
