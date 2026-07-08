#include <func_minimization/DFAmin.hpp> // публичные include подключаем как системные
#include <iostream>   // для std::cout
#include <vector> 

using namespace std;


vector<vector<char>> func_minimization::DFAmin(
    const vector<char>& Q, 
    const vector<char>& F,
    const unordered_map<char, unordered_map<char, char>>& transitions) {
    //vector<vector<char>> P = {Q[1:2], Q[3:end]};
    vector<vector<char>> P;
    P.push_back(vector<char>(Q.begin(), Q.begin() + 3)); // элементы 1,2
    P.push_back(vector<char>(Q.begin() + 3, Q.end()));       // элементы 3..конец
    return P;
}
