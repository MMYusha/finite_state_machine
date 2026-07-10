#include <func_transition/transit.hpp> // публичные include подключаем как системные
#include <iostream>   // для std::cout
#include <vector>   // для std::cout

using namespace std;

template <typename State, typename Symbol>

State func_transition::transit(const vector<Symbol>& input, State current,
              const std::unordered_map<State, std::unordered_map<Symbol, State>>& transitions) {
    // Обработка каждого символа
    for (Symbol ch : input) {   
        // Ищем таблицу переходов из текущего состояния
        auto state_it = transitions.find(current);
        if (state_it == transitions.end()) {
            std::cout << "Нет переходов из состояния " << current << "\n";
            break; // автомат застрял
        }
        // Ищем переход по текущему символу
        // Внутренняя таблица – это unordered_map<Symbol, State>
        auto sym_it = state_it->second.find(ch);
        if (sym_it == state_it->second.end()) {
            std::cout << "Нет перехода по символу '" << ch << "'\n";
            break; // переход не определён
        }

        // Выполняем переход
        current = sym_it->second;
        std::cout << "Переход по '" << ch << "' → состояние " << current << "\n";
    }
    std::cout << "\n";
    return current;
}

template string func_transition::transit<string, string>(
    const vector<string>&,
    string,
    const std::unordered_map<string, std::unordered_map<string, string>>&);