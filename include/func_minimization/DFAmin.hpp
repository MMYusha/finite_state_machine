#ifndef func_minimization_DFAmin_HPP_
#define func_minimization_DFAmin_HPP_

#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

namespace func_minimization{

vector<vector<char>> DFAmin(
    const vector<char>& alphabet,
    const vector<char>& Q, 
    const vector<char>& F,
    const unordered_map<char, unordered_map<char, char>>& transitions);
}

#endif // func_minimization_DFAmin_HPP_