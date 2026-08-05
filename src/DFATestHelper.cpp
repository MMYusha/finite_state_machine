#include <func_DFA/DFA.hpp> // публичные include подключаем как системные


namespace func_DFA{
using std::vector;
using std::string; 

vector<vector<string>> DFATestHelper::getPartition(const DFA5& dfa){
    HopcroftMinimizer minimizer(dfa);
    auto Partition = minimizer.computePartition();
    return Partition;   
}

DFA5 DFATestHelper::getNewDFAwithPartition(const DFA5& dfa, const vector<vector<string>>& Partition){
    return dfa.CreateNewDFAwithPartition(Partition);
}

}