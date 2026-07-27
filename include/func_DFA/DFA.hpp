#ifndef func_DFA_HPP_
#define func_DFA_HPP_

#include <unordered_map>
#include <vector>   

namespace func_DFA {

template <typename State, typename Symbol>
State transit(const std::vector<Symbol>& input, State current,
              const std::unordered_map<State, std::unordered_map<Symbol, State>>& transitions);

} // namespace func_transition

#endif // func_DFA_HPP_