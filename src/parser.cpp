#include "parser.hpp"

#include <stack>
#include <string>
#include <unordered_map>

std::string insert_concat_symbol(const std::string exp) {
  std::string with_concat_exp = "";

  char token, next_token;
  for (size_t i = 0; i < exp.size(); i++) {
    token = exp[i];
    with_concat_exp += token;

    if (token == '(' || token == '|') {
      continue;
    }

    if (i < exp.size() - 1) {
      next_token = exp[i + 1];

      if (next_token == '*' || next_token == '+' || next_token == '?' ||
          next_token == '|' || next_token == ')') {
        continue;
      }
      with_concat_exp += '.';
    }
  }

  return with_concat_exp;
}

std::unordered_map<char, int> OPERATOR_PRECEDENCE = {
    {'|', 0}, {'.', 1}, {'?', 2}, {'*', 2}, {'+', 2}};

std::string to_postfix(const std::string exp) {
  /* Implementation of the Shunting yard algorithm
   * https://en.wikipedia.org/wiki/Shunting_yard_algorithm
   */
  std::string postfix_exp = "";
  std::stack<char> op_stack;

  for (auto token : exp) {
    if (token == '.' || token == '|' || token == '?' || token == '*' ||
        token == '+') {
      while (!op_stack.empty() && op_stack.top() != '(' &&
             OPERATOR_PRECEDENCE[op_stack.top()] >=
                 OPERATOR_PRECEDENCE[token]) {
        postfix_exp += op_stack.top();
        op_stack.pop();
      }
      op_stack.push(token);
    } else if (token == '(' || token == ')') {
      if (token == '(') {
        op_stack.push(token);
      } else {
        while (op_stack.top() != '(') {
          postfix_exp += op_stack.top();
          op_stack.pop();
        }
        op_stack.pop();
      }
    } else {
      postfix_exp += token;
    }
  }

  while (!op_stack.empty()) {
    postfix_exp += op_stack.top();
    op_stack.pop();
  }

  return postfix_exp;
}
