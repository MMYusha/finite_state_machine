#ifndef func_DFA_HPP_
#define func_DFA_HPP_

#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

namespace func_DFA {

struct DFA {
    vector<string> string_transition;
    vector<string> permited_state;
    string start_state;
    vector<string> alphabet;
    vector<string> states;
    unordered_map<string, unordered_map<string, string>> transitions;
};


template <typename State, typename Symbol>
State transit(const std::vector<Symbol>& input, State current,
              const std::unordered_map<State, std::unordered_map<Symbol, State>>& transitions);

vector<vector<string>> DFAmin(
    const vector<string>& alphabet,
    const vector<string>& Q, 
    const vector<string>& F,
    const unordered_map<string, unordered_map<string, string>>& transitions);

DFA CreateNewTransitions(
    DFA OldDfa,
    vector<vector<string>> P);

DFA DFAinput(string filename);
void writeDFA(const std::string& filename, const DFA& dfa);




} // namespace func_transition

#endif // func_DFA_HPP_