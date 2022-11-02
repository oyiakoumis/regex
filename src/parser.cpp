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

std::string to_infix(const std::string postfix_exp) {
  std::stack<std::string> s;
  std::string c1, c2;

  for (auto token : postfix_exp) {
    if (token != '.' && token != '|' && token != '?' && token != '*' &&
        token != '+') {
      s.push(std::string(1, token));
    } else {
      if (token == '|' || token == '.') {
        c2 = s.top();
        s.pop();
        c1 = s.top();
        s.pop();
        if (token == '|') {
          s.push("(" + c1 + token + c2 + ")");
        } else {
          s.push(c1 + c2);
        }
      } else {
        c1 = s.top();
        s.pop();
        if (c1.size() == 1 || (c1[0] == '(' && c1.back() == ')')) {
          s.push(c1 + token);
        } else {
          s.push("(" + c1 + ")" + token);
        }
      }
    }
  }

  return s.top();
}
