#include <func_DFA/DFA.hpp> // публичные include подключаем как системные
#include <iostream>   
#include <vector>   

using namespace std;


string func_DFA::transit(const vector<string>& input, string current,
              const std::unordered_map<string, std::unordered_map<string, string>>& transitions) {
    // Обработка каждого символа
    for (string ch : input) {   
        // Ищем таблицу переходов из текущего состояния
        auto state_it = transitions.find(current);
        if (state_it == transitions.end()) {
            std::cout << "Нет переходов из состояния " << current << "\n";
            break; // автомат застрял
        }
        // Ищем переход по текущему символу
        // Внутренняя таблица – это unordered_map<string, string>
        auto sym_it = state_it->second.find(ch);
        if (sym_it == state_it->second.end()) {
            std::cout << "Нет перехода по символу '" << ch << "'\n";
            break; // переход не определён
        }

        // Выполняем переход
        current = sym_it->second;
        //std::cout << "Переход по '" << ch << "' → состояние " << current << "\n";
    }
    //std::cout << "\n";
    return current;
}

