#include <cstdio>  // для printf
#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>

#include <func_DFA/DFA.hpp>

using namespace std;
using namespace func_DFA;


int main() {
    system("chcp 65001 > nul"); // Для корректного вывода русского языка

    // ================= Пример использования ДКА =================
    auto dfa = DFABuilder{}.withCSV("input.csv").build(); // Чтение ДКА из csv файла
    dfa.print(); // Вывод ДКА в консоль
    dfa.transitFromCSV(); // Переход по строке, прочитанной в CSV 
    dfa.resetCurrentState(); // Сброс ДКА в начальное состояние
    dfa.transitInput({"0","1","0","1"}); // Переход по заданной строке
    dfa.minimize(); // Минимизация ДКА
    dfa.exportCSV("output.csv"); // Запись нового ДКА в файл




    // ================= Бенчмарк =================
    // параметры бенчмарка
    vector<int> vector_number_of_states;
    vector<int> vector_alphabet_size={2};
    string mode = "full";
    int repetitions=20;
    int seed=42;

    // массив размеров ДКА
    int min_states = 1;
    int max_states = 1024;
    int intermediate_number = 5;

    // создание бенчмарка
    auto bench = benchmarkBuilder{}.withInput(min_states, max_states, intermediate_number,
            seed, repetitions, mode, vector_number_of_states, vector_alphabet_size).build(); 
    bench.print();
    bench.run_benchmark(); // запуск бенчмарка

    cout << "\nНажмите Enter, чтобы закрыть окно...";
    cin.get();  // ждёт нажатия Enter
    return 0;
}