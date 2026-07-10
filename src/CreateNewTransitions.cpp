#include <func_minimization/DFAmin.hpp> // публичные include подключаем как системные
#include <iostream>   // для std::cout
#include <vector> 
#include <unordered_map>

using namespace std;
using namespace func_minimization;

unordered_map<string, unordered_map<string, string>> CreateNewTransitions(
    const unordered_map<string, unordered_map<string, string>>& oldTransitions,
    vector<vector<string>> P){
    
    unordered_map<string, unordered_map<string, string>> NewTransitions;
    NewTransitions = oldTransitions;

    return NewTransitions;
    }