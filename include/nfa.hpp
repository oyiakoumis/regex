#ifndef NFA_HPP
#define NFA_HPP

#include <memory>
#include <ostream>
#include <string>
#include <unordered_map>
#include <vector>

class Nfa;

class NfaState {
 private:
  std::vector<std::weak_ptr<NfaState>> w_epsilon_transitions;
  std::vector<std::shared_ptr<NfaState>> s_epsilon_transitions;
  std::unordered_map<char, std::shared_ptr<NfaState>> symbol_transitions;

 public:
  void add_w_epsilon_transition(std::shared_ptr<NfaState>);
  void add_s_epsilon_transition(std::shared_ptr<NfaState>);
  void add_symbol_transition(std::shared_ptr<NfaState>, const char);

  bool is_end() {
    return w_epsilon_transitions.empty() && s_epsilon_transitions.empty() &&
           symbol_transitions.empty();
  }
  friend void add_next_states(
      std::shared_ptr<NfaState> pstate,
      std::vector<std::shared_ptr<NfaState>>& next_pstates,
      std::vector<std::shared_ptr<NfaState>>& visited);

  friend bool search(Nfa nfa, const std::string exp);
};

class Nfa {
 private:
  std::shared_ptr<NfaState> head;
  std::shared_ptr<NfaState> tail;
  std::string representation;

 public:
  Nfa();
  Nfa(char);
  Nfa(const std::string);

  Nfa& make_union(Nfa&);
  Nfa& concat(Nfa&);
  Nfa& zero_or_one();
  Nfa& one_or_more();
  Nfa& closure();

  friend bool search(Nfa nfa, const std::string exp);
  friend std::ostream& operator<<(std::ostream&, const Nfa&);
};

#endif /* ifndef NFA_HPP */
