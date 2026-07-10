#include <cstdio>  // для printf
#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>

#include <func_transition/transit.hpp>
#include <func_minimization/DFAmin.hpp>
#include <func_input/DFAinput.hpp>

using namespace std;
using namespace func_transition;
using namespace func_minimization;
using namespace func_input;


int main() {
    system("chcp 65001 > nul"); 
    // Типы для читаемости
    using State = string;
    using Symbol = string;


    // Чтение ДКА из csv файла
    string filename = "input.csv";
    Result res;
    res = DFAinput(filename);


    // Информация о ДКА из файла
    cout << "\nСтрока перехода - ";
    for (auto cls : res.string_transition) {   
        printf("%s ", cls.c_str());
    }
    printf("\n");

    cout << "Допустимые состояния - ";
    for (auto cls : res.permited_state) {   
        printf("%s ", cls.c_str());
    }
    printf("\n");

    cout << "Начальное состояние - " << res.start_state << "\n";

    cout << "Алфавит - ";
    for (auto cls : res.alphabet) {   
        printf("%s ", cls.c_str());
    }
    printf("\n\n");


    // Инициализация ДКА
    State current = res.start_state;
    vector<string> F = res.permited_state;
    vector<string> input = res.string_transition; // пример входной строки из файла
    vector<string> E = res.alphabet;
    vector<string> Q = res.states;
    unordered_map<string, unordered_map<string, string>> transitions = res.transitions;


    // Обработка строки перехода
    State final_state = transit(input, current, transitions);
    cout << "Переход в состояние" << final_state << " из начального "<< res.start_state  << "\n\n";

    
    // Минимизация ДКА
    vector<vector<string>> P = DFAmin(E,Q,F,transitions);
    printf("Разбиения:\n");
    for (auto cls : P) {
        printf("{ ");
        for (auto state : cls) {
            printf("%s ", state.c_str());
        }
        printf("}\n");
    }
    printf("\n");

    writeDFA("output.csv", res);

    return 0;
}