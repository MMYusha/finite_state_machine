#include <func_DFA/DFA.hpp> // публичные include подключаем как системные
#include <iostream>   
#include <vector>   

using namespace std;


namespace func_DFA{

    
void DFA5::transit_string(const vector<string>& str) {
    cout << "\n===== Осуществление перехода ====="<< endl;
    cout << "Переход из состояния - " << current_state << endl;
    cout << "По строке - "; 
    auto flag = true;
    // Обработка каждого символа
    for (string ch : str) {   
        // Вывод строки перехода
        if (flag) {
            cout << ch;
            flag = false;
        }
        else{
            cout << ", " << ch;
        }
        
        // Ищем таблицу переходов из текущего состояния
        auto state_it = transitions.find(current_state);
        if (state_it == transitions.end()) {
            std::cout << "\n-----------Нет переходов из состояния " << current_state << "---------------\n";
            break; // автомат застрял
        }
        // Ищем переход по текущему символу
        // Внутренняя таблица – это unordered_map<string, string>
        auto sym_it = state_it->second.find(ch);
        if (sym_it == state_it->second.end()) {
            std::cout << "\n------------Нет перехода по символу '" << ch << "'-------------------\n";
            break; // переход не определён
        }

        // Выполняем переход
        current_state = sym_it->second;
        //std::cout << "\nПереход по '" << ch << "' → состояние " << current_state << "\n";
    }
    cout << "\nВ состояние - " << current_state << endl;
    cout << endl;

}


void DFA5::transit_Input(const vector<string>& input){
    transit_string(input);
}

void DFA5::transit_fromCSV(){
    transit_string(string_transition);
}

}
