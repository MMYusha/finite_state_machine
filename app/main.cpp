#include <cstdio>  // для printf
#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>
#include <func_transition/transit.hpp>
#include <func_minimization/DFAmin.hpp>

using namespace std;
using namespace func_transition;
using namespace func_minimization;

int main() {
    system("chcp 65001 > nul"); 
    // Типы для читаемости
    using State = char;
    using Symbol = char;

    // Вложенная хеш-таблица переходов:
    // состояние -> (символ -> следующее состояние)
    unordered_map<State, unordered_map<Symbol, State>> transitions;

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


    vector<Symbol> E = {'0','1'};
    vector<State> Q = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'}; // множество состояний
    vector<State> F = {'C'}; // множество допустимы состояний


    State start = 'A';  // начальное состояние
    State current = start;

    string input = "0101"; // пример входной строки
    // Обработка строки
    State final_state = transit(input, current, transitions);
    cout << "Переход состояние " << final_state << " из начального "<< start <<" по строке " << input << "\n";

    vector<vector<char>> P = DFAmin(E,Q,F,transitions);
    for (auto cls : P) {
        printf("{ ");
        for (auto state : cls) {
            printf("%c ", state);
        }
        printf("}\n");
    }
    return 0;
}