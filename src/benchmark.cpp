#include <func_DFA/DFA.hpp>  

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
// Генератор ДКА 
// ------------------------------------------------------------


namespace func_DFA{


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
    vector<int> vector_number_of_states,
    vector<int> vector_alphabet_size,
    string mode,
    int repetitions,
    int seed
) {
    cout << "\n======== Запуск Бенчмарка =======" << endl;
    vector<pair<int, double>> results;
    for (int number_of_states : vector_number_of_states) {
        double total_time = 0.0;
        for (int alphabet_size : vector_alphabet_size){
            for (int rep = 0; rep < repetitions; ++rep) {
                auto dfa = DFABuilder{}.generatedDFA(number_of_states, alphabet_size, mode, seed + rep).build();
                
                auto start = high_resolution_clock::now();
                dfa.minimize();
                auto end = high_resolution_clock::now();

                duration<double, milli> elapsed = end - start;
                total_time += elapsed.count();
            }
        }
        double avg = total_time / repetitions / vector_alphabet_size.size();
        results.push_back({number_of_states, avg});
        cout << "States: " << number_of_states << ", avg time: " << avg << " ms\n";
    }

    save_benchmark_data(results, "dfa_benchmark_data.txt");
    plot_with_gnuplot("dfa_benchmark_data.txt");
}
}