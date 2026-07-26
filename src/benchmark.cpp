#include <func_DFA/benchmark.hpp>
#include <func_DFA/DFAmin.hpp>  

#include <chrono>
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <random>
#include <algorithm>
#include <fstream>
#include <cstdlib>   // для system()

using namespace std;
using namespace std::chrono;

// ------------------------------------------------------------
// Генератор ДКА (скопирован из предыдущего ответа)
// ------------------------------------------------------------
struct GeneratedDFA {
    vector<string> states;
    vector<string> alphabet;
    unordered_map<string, unordered_map<string, string>> transitions;
    string start_state;
    vector<string> final_states;
};

namespace func_benchmark{
GeneratedDFA generate_dfa(int n, int alphabet_size, const string& mode, int seed) {
    // Проверки
    if (n < 2) throw invalid_argument("n должно быть не меньше 2");
    if (alphabet_size < 1) throw invalid_argument("alphabet_size должен быть положительным");
    if (mode != "sparse" && mode != "full")
        throw invalid_argument("mode должен быть 'sparse' или 'full'");

    // Генератор случайных чисел с фиксированным seed
    mt19937 rng(seed);

    // 1. Создаём состояния и алфавит
    vector<string> states;
    for (int i = 0; i < n; ++i)
        states.push_back("q" + to_string(i));

    vector<string> alphabet;
    for (int i = 0; i < alphabet_size; ++i)
        alphabet.push_back("a" + to_string(i));

    // 2. Инициализируем таблицу переходов (все внутренние карты пусты)
    unordered_map<string, unordered_map<string, string>> transitions;
    for (const auto& state : states)
        transitions[state] = {};

    // 3. Строим цепочку по первому символу алфавита (гарантирует достижимость всех состояний)
    for (int i = 0; i < n - 1; ++i) {
        transitions[states[i]][alphabet[0]] = states[i + 1];
    }

    // 4. Вычисляем целевую плотность переходов
    int capacity = n * alphabet_size;
    if (mode == "sparse" && capacity % 2 != 0) {
        throw invalid_argument("Для sparse режима n*|Sigma| должно быть чётным");
    }
    int target_m = (mode == "full") ? capacity : capacity / 2;
    if (target_m < n - 1) {
        throw invalid_argument("Целевая плотность недостаточна для достижимости всех состояний");
    }

    // 5. Формируем список доступных ячеек (state, symbol), в которых ещё нет перехода
    vector<pair<string, string>> available_keys;
    for (const auto& state : states) {
        for (const auto& sym : alphabet) {
            auto it_state = transitions.find(state);
            if (it_state != transitions.end() && it_state->second.find(sym) == it_state->second.end()) {
                available_keys.emplace_back(state, sym);
            }
        }
    }

    // 6. Перемешиваем доступные ячейки
    shuffle(available_keys.begin(), available_keys.end(), rng);

    // 7. Добавляем недостающие переходы до target_m
    int current_count = n - 1;  // уже есть цепочка
    int to_add = target_m - current_count;
    uniform_int_distribution<> dist(0, n - 1);
    for (int i = 0; i < to_add && i < (int)available_keys.size(); ++i) {
        const auto& key = available_keys[i];
        string target_state = states[dist(rng)];
        transitions[key.first][key.second] = target_state;
    }

    // 8. Начальное и финальные состояния
    string start_state = states[0];
    vector<string> final_states = {states[n - 1]};  // только последнее состояние

    return {states, alphabet, transitions, start_state, final_states};
}

// ------------------------------------------------------------
// Сохранение данных и построение графика
// ------------------------------------------------------------
void save_benchmark_data(const vector<pair<int, double>>& data, const string& filename) {
    ofstream out(filename);
    out << "# states time_ms\n";
    for (const auto& p : data) {
        out << p.first << " " << p.second << "\n";
    }
}

void plot_with_gnuplot(const string& data_file, const string& output_png = "dfa_min_time.png") {
    string script = "plot_script.gp";
    ofstream script_file(script);
    script_file << "set terminal png size 800,600\n";
    script_file << "set output '" << output_png << "'\n";
    script_file << "set xlabel 'Number of states'\n";
    script_file << "set ylabel 'Execution time (ms)'\n";
    script_file << "set title 'DFA minimization performance'\n";
    script_file << "set grid\n";
    script_file << "plot '" << data_file << "' using 1:2 with linespoints title 'DFAmin'\n";
    script_file.close();

    string cmd = "gnuplot -persist " + script;
    int ret = system(cmd.c_str());
    if (ret != 0) {
        cerr << "Gnuplot execution failed. Make sure gnuplot is installed and in PATH.\n";
    } else {
        cout << "Graph saved as " << output_png << "\n";
    }
}

// ------------------------------------------------------------
// Основная функция бенчмарка
// ------------------------------------------------------------
void run_benchmark(
    vector<int> number_of_states,
    vector<int> vector_alphabet_size,
    string mode,
    int repetitions,
    int seed
) {
    vector<pair<int, double>> results;
    for (int n : number_of_states) {
        double total_time = 0.0;
        for (int alphabet_size : vector_alphabet_size){
            for (int rep = 0; rep < repetitions; ++rep) {
                auto dfa = generate_dfa(n, alphabet_size, mode, seed + rep);
                const auto& Q = dfa.states;
                const auto& alphabet = dfa.alphabet;
                const auto& F = dfa.final_states;
                const auto& transitions = dfa.transitions;

                auto start = high_resolution_clock::now();
                auto P = func_minimization::DFAmin(alphabet, Q, F, transitions);
                auto end = high_resolution_clock::now();

                duration<double, milli> elapsed = end - start;
                total_time += elapsed.count();
            }
        }
        double avg = total_time / repetitions / vector_alphabet_size.size();
        results.push_back({n, avg});
        cout << "States: " << n << ", avg time: " << avg << " ms\n";
    }

    save_benchmark_data(results, "dfa_benchmark_data.txt");
    plot_with_gnuplot("dfa_benchmark_data.txt");
}
}