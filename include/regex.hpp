#ifndef REGEX_HPP
#define REGEX_HPP

#include <string>

#include "nfa.hpp"

class Matcher {
 private:
  Nfa nfa;

 public:
  Matcher(std::string exp) : nfa{Nfa(exp)} {}
  bool exact_match(const std::string exp);
};
#endif /* ifndef REGEX_HPP */
