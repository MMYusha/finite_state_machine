#ifndef func_minimization_DFAmin_HPP_
#define func_minimization_DFAmin_HPP_

#include <func_input/DFAinput.hpp>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

namespace func_minimization{


vector<vector<string>> DFAmin(
    const vector<string>& alphabet,
    const vector<string>& Q, 
    const vector<string>& F,
    const unordered_map<string, unordered_map<string, string>>& transitions);

func_input::Result CreateNewTransitions(
    func_input::Result OldRes,
    vector<vector<string>> P);
}
#endif // func_minimization_DFAmin_HPP_