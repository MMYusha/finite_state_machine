#include <iostream>
#include <unordered_map>
#include <string>
#include <func_transition/transit.hpp>

int main() {
    system("chcp 65001 > nul"); 
    // Типы для читаемости
    using State = char;
    using Symbol = char;

    // Вложенная хеш-таблица переходов:
    // состояние -> (символ -> следующее состояние)
    std::unordered_map<State, std::unordered_map<Symbol, State>> transitions;

    // Заполнение переходов согласно таблице
    transitions['A']['0'] = 'B';
    transitions['A']['1'] = 'F';

    transitions['B']['0'] = 'G';
    transitions['B']['1'] = 'C';

    transitions['C']['0'] = 'A';
    transitions['C']['1'] = 'C';

    transitions['D']['0'] = 'C';
    transitions['D']['1'] = 'G';

    transitions['E']['0'] = 'H';
    transitions['E']['1'] = 'F';

    transitions['F']['0'] = 'C';
    transitions['F']['1'] = 'G';

    transitions['G']['0'] = 'G';
    transitions['G']['1'] = 'E';

    transitions['H']['0'] = 'G';
    transitions['H']['1'] = 'C';

    State start = 'A';  // начальное состояние
    State current = start;

    std::string input = "0101"; // пример входной строки

    // Обработка строки
    State final_state = func_transition::transit(input, current, transitions);
    std::cout << "Переход состояние " << final_state << " из начального "<< start <<" по строке " << input << "\n";
    return 0;
}