#include <func_DFA/DFA.hpp> // публичные include подключаем как системные
#include <iostream>   // для std::cout
#include <vector> 
#include <string> 
#include <unordered_map>
#include <algorithm> 

#include <func_DFA/DFA.hpp>

using namespace std;


func_DFA::DFA func_DFA::CreateNewTransitions(
    func_DFA::DFA OldDfa,
    vector<vector<string>> P) {

    // отображение "старое состояние → имя класса" 
    unordered_map<string, string> stateToNewState;
    vector<string> newStates;

    for (auto cls : P) {
        // Формируем имя класса как сумму состояний через '+'
        string className;
        for (size_t i = 0; i < cls.size(); ++i) {
            if (i > 0) className += "+";
            className += cls[i];
        }
        newStates.push_back(className);
        for (string state : cls) {
            stateToNewState[state] = className;   // запоминаем соответствие
        }
    }

    // построение новых переходов 
    unordered_map<string, unordered_map<string, string>> newTransitions;

    for (auto cls : P) {
        string className = stateToNewState[cls[0]]; // имя текущего класса

        unordered_map<string, string> trans; // переходы для этого класса
        for (string sym : OldDfa.alphabet) {
            // Берем переход из любого состояния класса (возьмём первое)
            string oldTarget = OldDfa.transitions[cls[0]][sym];
            // Находим класс, в который попадаем
            string newTarget = stateToNewState[oldTarget];
            trans[sym] = newTarget;
        }
        newTransitions[className] = trans;
    }

    // Обновление таблицы переходов
    func_DFA::DFA NewDfa = OldDfa;
    NewDfa.states = newStates;
    NewDfa.transitions = newTransitions;

    
    // Обновление начального состояния
    NewDfa.start_state = stateToNewState[OldDfa.start_state]; // имя текущего класса


    // Обновление допустимых состояний
    vector<string> newPermited;
    for (string st : OldDfa.permited_state) {
        auto it = stateToNewState.find(st);
        if (it != stateToNewState.end()) {
            // добавляем только уникальные классы
            if (find(newPermited.begin(), newPermited.end(), it->second) == newPermited.end())
                newPermited.push_back(it->second);
        }
    }
    NewDfa.permited_state = newPermited;


    return NewDfa;
}