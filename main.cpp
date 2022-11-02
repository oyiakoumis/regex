#include <iostream>
#include <string>

#include "regex.hpp"

int main(int argc, char *argv[]) {
  std::string exp1, exp2;
  if (argc != 3) {
    exp1 = "(a|b)*c";
    exp2 = "abc";
  } else {
    exp1 = argv[1];
    exp2 = argv[2];
  }

  Matcher matcher(exp1);
  std::cout << (matcher.exact_match(exp2) ? "true" : "false") << std::endl;

  return 0;
}
