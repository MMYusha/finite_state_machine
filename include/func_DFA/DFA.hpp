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
State transit(const vector<Symbol>& input, State current,
              const unordered_map<State, unordered_map<Symbol, State>>& transitions);

vector<vector<string>> DFAmin(
    const vector<string>& alphabet,
    const vector<string>& Q, 
    const vector<string>& F,
    const unordered_map<string, unordered_map<string, string>>& transitions);

DFA CreateNewTransitions(
    DFA OldDfa,
    vector<vector<string>> P);

DFA DFAinput(string filename);
void writeDFA(const string& filename, const DFA& dfa);

void run_benchmark(
        vector<int> number_of_states,
        vector<int> vector_alphabet_size,
        string = "full",
        int repetitions = 3,
        int seed = 42
    );


} // namespace func_transition

#endif // func_DFA_HPP_