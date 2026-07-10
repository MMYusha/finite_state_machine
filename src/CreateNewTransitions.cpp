#include <func_minimization/DFAmin.hpp> // публичные include подключаем как системные
#include <iostream>   // для std::cout
#include <vector> 
#include <string> 
#include <unordered_map>
#include <algorithm> 

#include <func_input/DFAinput.hpp>

using namespace std;


func_input::Result func_minimization::CreateNewTransitions(
    func_input::Result OldRes,
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
        for (string sym : OldRes.alphabet) {
            // Берем переход из любого состояния класса (возьмём первое)
            string oldTarget = OldRes.transitions[cls[0]][sym];
            // Находим класс, в который попадаем
            string newTarget = stateToNewState[oldTarget];
            trans[sym] = newTarget;
        }
        newTransitions[className] = trans;
    }

    // Обновление таблицы переходов
    func_input::Result NewRes = OldRes;
    NewRes.states = newStates;
    NewRes.transitions = newTransitions;

    
    // Обновление начального состояния
    NewRes.start_state = stateToNewState[OldRes.start_state]; // имя текущего класса


    // Обновление допустимых состояний
    vector<string> newPermited;
    for (string st : OldRes.permited_state) {
        auto it = stateToNewState.find(st);
        if (it != stateToNewState.end()) {
            // добавляем только уникальные классы
            if (find(newPermited.begin(), newPermited.end(), it->second) == newPermited.end())
                newPermited.push_back(it->second);
        }
    }
    NewRes.permited_state = newPermited;


    return NewRes;
}