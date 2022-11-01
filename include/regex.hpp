#ifndef REGEX_HPP
#define REGEX_HPP

#include <string>

#include "nfa.hpp"

bool search(Nfa nfa, const std::string exp);

#endif /* ifndef REGEX_HPP */
