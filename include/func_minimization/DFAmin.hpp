#ifndef func_minimization_DFAmin_HPP_
#define func_minimization_DFAmin_HPP_

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
}

#endif // func_minimization_DFAmin_HPP_