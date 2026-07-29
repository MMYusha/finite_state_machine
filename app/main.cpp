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

    // ================= Пример использования ДКА =================
    auto dfa = DFABuilder{}.withCSV("input.csv").build(); // Чтение ДКА из csv файла
    dfa.print(); // Вывод ДКА в консоль
    dfa.transit_fromCSV(); // Переход по строке, прочитанной в CSV 
    dfa.transit_Input({"0","1","0","1"}); // Переход по заданной строке
    dfa.minimize(); // Минимизация ДКА
    dfa.exportCSV("output.csv"); // Запись нового ДКА в файл




    // ================= Бенчмарк =================
    // параметры бенчмарка
    vector<int> number_of_states;
    vector<int> vector_alphabet_size={2};
    string mode = "full";
    int repetitions=20;
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

    cout << "\nНажмите Enter, чтобы закрыть окно...";
    cin.get();  // ждёт нажатия Enter
    return 0;
}