#include <cstdio>  // для printf
#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>

#include <func_DFA/DFA.hpp>

using namespace std;
using namespace func_DFA;


int main() {
    system("chcp 65001 > nul"); 
    // Типы для читаемости
    using State = string;
    using Symbol = string;


    // Чтение ДКА из csv файла
    string filename = "input.csv";
    DFA dfa;
    dfa = DFAinput(filename);


    // Информация о ДКА из файла
    cout << "\nСтрока перехода - ";
    for (auto cls : dfa.string_transition) {   
        printf("%s ", cls.c_str());
    }
    printf("\n");

    cout << "Допустимые состояния - ";
    for (auto cls : dfa.permited_state) {   
        printf("%s ", cls.c_str());
    }
    printf("\n");

    cout << "Начальное состояние - " << dfa.start_state << "\n";

    cout << "Алфавит - ";
    for (auto cls : dfa.alphabet) {   
        printf("%s ", cls.c_str());
    }
    printf("\n\n");


    // Инициализация ДКА
    State current = dfa.start_state;
    vector<string> F = dfa.permited_state;
    vector<string> input = dfa.string_transition; // пример входной строки из файла
    vector<string> E = dfa.alphabet;
    vector<string> Q = dfa.states;
    unordered_map<string, unordered_map<string, string>> transitions = dfa.transitions;


    // Обработка строки перехода
    State final_state = transit(input, current, transitions);
    cout << "Переход в состояние " << final_state << " из начального "<< dfa.start_state  << "\n\n";

    
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

    dfa = CreateNewTransitions(dfa, P);
    writeDFA("output.csv", dfa);


    // ================= Фактическая сложность =================
    // параметры бенчмарка
    vector<int> number_of_states;
    vector<int> vector_alphabet_size={2,3,4,5,6,7,8,9,10};
    string mode = "full";
    int repetitions=10;
    int seed=42;

    // массив размеров ДКА
    int min_states = 1;
    int max_states = 1024;
    int intermediate_number = 5;
    int n = min_states;
    // увеличение размеров ДКА в зависимости n**2
    while (n < max_states) {
        if (n == 1) {
            n = 2;
        }          // если начали с 1, следующая степень 2
        else n *= 2;
        
        // промежуточные значения
        if (!number_of_states.empty()){;
            int last = number_of_states.back();
            int step = (n - last)/intermediate_number;
            if (number_of_states.back() + step*(intermediate_number-1) < n && step > 0){
                for (int count = 0; count < intermediate_number-1; ++count){
                    number_of_states.push_back(number_of_states.back() + step);
                }
            }
        }
        number_of_states.push_back(n);

    }

    run_benchmark(number_of_states, vector_alphabet_size, mode, repetitions, seed);

    cout << "Нажмите Enter, чтобы закрыть окно...";
    cin.get();  // ждёт нажатия Enter
    return 0;
}