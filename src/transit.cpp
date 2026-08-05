#include <func_DFA/DFA.hpp> // публичные include подключаем как системные

#include <iostream>   

namespace func_DFA{
using std::vector;
using std::string;
using std::cout; 
using std::endl; 
    
void DFA5::transit_string() {
    cout << "\n===== Осуществление перехода ====="<< endl;
    printCurrentState();
    printStringTransition();

    // Обработка каждого символа в строке перехода
    bool flag = true;
    for (string symbol : string_transition) {    
        // Ищем таблицу переходов из текущего состояния
        // Проверка, что текущее состояние есть в таблице переходов, т.е. что у из него есть переходы
        flag = stateIt();

        // Ищем переход по текущему символу
        // Проверка что существует переход 
        flag = SymIt(symbol);

        // Выполняем переход
        if (flag)
            current_state = transition(current_state, symbol);
        else break;
    }
    cout << "Новое ";
    printCurrentState();
    
    cout << endl;

}

bool DFA5::stateIt(){  
    // Ищем таблицу переходов из текущего состояния
    auto state_it = transitions.find(current_state);

    // Проверка, что текущее состояние есть в таблице переходов, т.е. что у из него есть переходы
    if (state_it == transitions.end()) {
        // автомат застрял
        cout << "-----------Нет переходов из состояния " << current_state << "---------------\n";
        return false;
    }    

    return true;
}

bool DFA5::SymIt(string symbol){  
    // Ищем переход по текущему символу
    // Внутренняя таблица – это unordered_map<string, string>
    auto state_it = transitions.find(current_state);
    auto sym_it = state_it->second.find(symbol);
    
    if (sym_it == state_it->second.end()) {
        // переход не определён
        cout << "\n------------Нет перехода по символу '" << symbol << "'-------------------\n";
        return false;
    } 

    return true;
}


string DFA5::transition(const string& state, const string& symbol){
    string NewState = transitions[state][symbol];

    return NewState;
}


void DFA5::transitInput(const vector<string>& input){
    string_transition = input;
    transit_string();
}


void DFA5::transitFromCSV(){
    transit_string();
}

}
