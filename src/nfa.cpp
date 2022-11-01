#include "nfa.hpp"

#include <iostream>
#include <memory>
#include <stack>
#include <string>

#include "parser.hpp"

std::ostream& operator<<(std::ostream& out, const Nfa& nfa) {
  out << "N(" << nfa.representation << ")";
  return out;
}
void NfaState::add_w_epsilon_transition(std::shared_ptr<NfaState> state) {
  w_epsilon_transitions.push_back(state);
}

void NfaState::add_s_epsilon_transition(std::shared_ptr<NfaState> state) {
  s_epsilon_transitions.push_back(state);
}

void NfaState::add_symbol_transition(std::shared_ptr<NfaState> state,
                                     const char symbol) {
  symbol_transitions[symbol] = state;
}

Nfa::Nfa() {
  head = std::make_shared<NfaState>();
  tail = std::make_shared<NfaState>();
  head->add_s_epsilon_transition(tail);
  representation = "";
}

Nfa::Nfa(const char symbol) {
  head = std::make_shared<NfaState>();
  tail = std::make_shared<NfaState>();
  head->add_symbol_transition(tail, symbol);
  representation = symbol;
}

Nfa::Nfa(const std::string exp) {
  std::string with_concat_exp = insert_concat_symbol(exp);
  std::string postfix_exp = to_postfix(with_concat_exp);

  std::stack<Nfa> nfa_stack;
  Nfa nfa1, nfa2;

  for (auto token : postfix_exp) {
    if (token == '*') {
      nfa1 = nfa_stack.top().closure();
      nfa_stack.pop();
      nfa_stack.push(nfa1);
    } else if (token == '?') {
      nfa1 = nfa_stack.top().zero_or_one();
      nfa_stack.pop();
      nfa_stack.push(nfa1);
    } else if (token == '+') {
      nfa1 = nfa_stack.top().one_or_more();
      nfa_stack.pop();
      nfa_stack.push(nfa1);
    } else if (token == '|') {
      nfa2 = nfa_stack.top();
      nfa_stack.pop();
      nfa1 = nfa_stack.top();
      nfa_stack.pop();
      nfa_stack.push(nfa1.make_union(nfa2));
    } else if (token == '.') {
      nfa2 = nfa_stack.top();
      nfa_stack.pop();
      nfa1 = nfa_stack.top();
      nfa_stack.pop();
      nfa_stack.push(nfa1.concat(nfa2));
    } else {
      nfa_stack.push(Nfa(token));
    }
  }

  *this = nfa1;
}

Nfa& Nfa::make_union(Nfa& other_nfa) {
  std::shared_ptr<NfaState> start = std::make_shared<NfaState>();
  std::shared_ptr<NfaState> end = std::make_shared<NfaState>();

  start->add_s_epsilon_transition(this->head);
  start->add_s_epsilon_transition(other_nfa.head);
  this->head = start;

  this->tail->add_s_epsilon_transition(end);
  other_nfa.tail->add_s_epsilon_transition(end);
  this->tail = end;

  this->representation =
      "(" + this->representation + "|" + other_nfa.representation + ")";

  return *this;
}

Nfa& Nfa::concat(Nfa& other_nfa) {
  this->tail->add_s_epsilon_transition(other_nfa.head);
  this->tail = other_nfa.tail;

  this->representation = this->representation + "." + other_nfa.representation;

  return *this;
}

Nfa& Nfa::zero_or_one() {
  std::shared_ptr<NfaState> start = std::make_shared<NfaState>();
  std::shared_ptr<NfaState> end = std::make_shared<NfaState>();

  start->add_s_epsilon_transition(this->head);
  start->add_w_epsilon_transition(end);
  this->tail->add_s_epsilon_transition(end);

  this->head = start;
  this->tail = end;

  this->representation = this->representation + "?";

  return *this;
}

Nfa& Nfa::one_or_more() {
  std::shared_ptr<NfaState> start = std::make_shared<NfaState>();
  std::shared_ptr<NfaState> end = std::make_shared<NfaState>();

  start->add_s_epsilon_transition(this->head);
  this->tail->add_w_epsilon_transition(this->head);
  this->tail->add_s_epsilon_transition(end);

  this->head = start;
  this->tail = end;

  this->representation = this->representation + "+";

  return *this;
}

Nfa& Nfa::closure() {
  std::shared_ptr<NfaState> start = std::make_shared<NfaState>();
  std::shared_ptr<NfaState> end = std::make_shared<NfaState>();

  start->add_s_epsilon_transition(this->head);
  start->add_w_epsilon_transition(end);
  this->tail->add_w_epsilon_transition(this->head);
  this->tail->add_s_epsilon_transition(end);

  this->head = start;
  this->tail = end;

  this->representation = this->representation + "*";

  return *this;
}
