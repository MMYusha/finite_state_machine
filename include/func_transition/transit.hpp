#ifndef func_transition_transit_HPP_
#define func_transition_transit_HPP_

#include <string>
#include <unordered_map>
#include <vector>   // для std::cout

namespace func_transition {

template <typename State, typename Symbol>
State transit(const std::vector<Symbol>& input, State current,
              const std::unordered_map<State, std::unordered_map<Symbol, State>>& transitions);

} // namespace func_transition

#endif // func_transition_transit_HPP_