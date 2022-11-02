#include "regex.hpp"

#include <algorithm>
#include <memory>
#include <vector>

void add_next_states(std::shared_ptr<NfaState> pstate,
                     std::vector<std::shared_ptr<NfaState>>& next_pstates,
                     std::vector<std::shared_ptr<NfaState>>& visited) {
  std::vector<std::shared_ptr<NfaState>> epsilon_transitions =
      pstate->s_epsilon_transitions;

  std::vector<std::shared_ptr<NfaState>> w_epsilon_transitions(
      pstate->w_epsilon_transitions.size());

  std::transform(pstate->w_epsilon_transitions.begin(),
                 pstate->w_epsilon_transitions.end(),
                 w_epsilon_transitions.begin(),
                 [](std::weak_ptr<NfaState> ptr) { return ptr.lock(); });

  epsilon_transitions.insert(epsilon_transitions.end(),
                             w_epsilon_transitions.begin(),
                             w_epsilon_transitions.end());

  if (!epsilon_transitions.empty()) {
    NfaState next_pstate;
    for (auto next_pstate : epsilon_transitions) {
      if (std::find(visited.begin(), visited.end(), next_pstate) ==
          visited.end()) {
        visited.push_back(next_pstate);
        add_next_states(next_pstate, next_pstates, visited);
      }
    }
  } else {
    next_pstates.push_back(pstate);
  }
}

bool Matcher::exact_match(const std::string exp) {
  std::vector<std::shared_ptr<NfaState>> current_pstates, next_pstates;
  std::shared_ptr<NfaState> next_pstate;
  std::vector<std::shared_ptr<NfaState>> visited;

  add_next_states(nfa.head, current_pstates, visited);
  visited.clear();

  for (auto symbol : exp) {
    next_pstates.clear();

    for (auto pstate : current_pstates) {
      next_pstate = pstate->symbol_transitions[symbol];

      if (next_pstate) {
        add_next_states(next_pstate, next_pstates, visited);
        visited.clear();
      }
    }

    current_pstates = next_pstates;
  }

  auto it = std::find(current_pstates.begin(), current_pstates.end(), nfa.tail);

  return it != current_pstates.end();
}
