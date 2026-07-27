#ifndef func_DFA_HPP_
#define func_DFA_HPP_

#include <string>
#include <vector>
#include <unordered_map>

#include <func_DFA/DFAinput.hpp>


namespace func_DFA {

template <typename State, typename Symbol>
State transit(const std::vector<Symbol>& input, State current,
              const std::unordered_map<State, std::unordered_map<Symbol, State>>& transitions);

vector<vector<string>> DFAmin(
    const vector<string>& alphabet,
    const vector<string>& Q, 
    const vector<string>& F,
    const unordered_map<string, unordered_map<string, string>>& transitions);

func_input::Result CreateNewTransitions(
    func_input::Result OldRes,
    vector<vector<string>> P);







} // namespace func_transition

#endif // func_DFA_HPP_