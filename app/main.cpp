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
    
    // Вложенная хеш-таблица переходов:
    // состояние -> (символ -> следующее состояние)
    unordered_map<string, unordered_map<string, string>> transitions;

    // Заполнение переходов
    transitions["A"]["0"] = "B";
    transitions["A"]["1"] = "F";

    transitions["B"]["0"] = "G";
    transitions["B"]["1"] = "C";

    transitions["C"]["0"] = "A";
    transitions["C"]["1"] = "C";

    transitions["D"]["0"] = "C";
    transitions["D"]["1"] = "G";

    transitions["E"]["0"] = "H";
    transitions["E"]["1"] = "F";

    transitions["F"]["0"] = "C";
    transitions["F"]["1"] = "G";

    transitions["G"]["0"] = "G";
    transitions["G"]["1"] = "E";

    transitions["H"]["0"] = "G";
    transitions["H"]["1"] = "C";

    // Алфавит, состояния, допускающие, начальное состояние — все как строки
    vector<string> E = {"0", "1"};
    vector<string> Q = {"A", "B", "C", "D", "E", "F", "G", "H"};
    vector<string> F = {"C"};

    string start = "A";
    State current = start;

    string filename = "input.csv";
    Result res;
    res = DFAinput(filename);
    cout << "Строка перехода - ";
    for (auto cls : res.string_transition) {   
        printf("%s ", cls.c_str());
    }
    printf("\n");

    vector<string> input = res.string_transition; // пример входной строки из файла
    // Обработка строки
    State final_state = transit(input, current, transitions);
    cout << "Переход состояние " << final_state << " из начального "<< start  << "\n";

    vector<vector<string>> P = DFAmin(E,Q,F,transitions);
    for (auto cls : P) {
        printf("{ ");
        for (auto state : cls) {
            printf("%s ", state.c_str());
        }
        printf("}\n");
    }



    return 0;
}