#include <func_DFA/DFA.hpp> // публичные include подключаем как системные
#include <vector>
#include <string>
#include <unordered_map>
#include <iostream>

using namespace std;

namespace func_DFA{

vector<vector<string>> DFATestHelper::getPartition(const DFA5& dfa){
    return dfa.computePartition();   
}

DFA5 DFATestHelper::getNewDFAwithPartition(const DFA5& dfa, const vector<vector<string>>& Partition){
    return dfa.CreateNewTransitions(Partition);
}

}