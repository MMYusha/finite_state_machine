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

benchmark::benchmark(
    const vector<int> vector_alph_size,
    const vector<int> vector_number_of_st,
    const string mod,
    const int repetit,
    const int sed
) : vector_alphabet_size(vector_alph_size), vector_number_of_states(vector_number_of_st), 
    mode(mod), repetitions(repetit), seed(sed) {}

// ------------------------------------------------------------
// Сохранение данных и построение графика
// ------------------------------------------------------------
void benchmark::save_benchmark_data(const vector<pair<int, double>>& data, const string& filename) {
    ofstream out(filename);
    out << "# states time_ms\n";
    for (const auto& p : data) {
        out << p.first << " " << p.second << "\n";
    }
}

void benchmark::plot_with_gnuplot(const string& data_file, const string& output_png) {
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
void benchmark::run_benchmark() {
    cout << "\n======== Запуск Бенчмарка =======" << endl;
    
    vector<pair<int, double>> results;
    for (int number_of_states : vector_number_of_states) {
        double total_time = 0.0;
        for (int alphabet_size : vector_alphabet_size){
            for (int rep = 0; rep < repetitions; ++rep) {
                auto dfa = DFABuilder{}.generatedDFA(number_of_states, alphabet_size, mode, seed + rep).build();
                
                auto start = high_resolution_clock::now();
                auto Partition =  DFATestHelper::getPartition(dfa);
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

void benchmark::print(){
    cout << "\n\n======= Информация о бенчмарке =======" << endl;

    cout << "Вектор размера алфавита - ";
    for (int alphabet_size : vector_alphabet_size){
        cout << alphabet_size << " ";
    }

    cout << "\nВектор количества состояний - ";
    for (int number_of_states : vector_number_of_states){
        cout << number_of_states << " ";
    }

    cout << "\nРежим - "<< mode <<endl;
    cout << "Повторения - "<< repetitions <<endl;
    cout << "Сид - " << seed <<endl;


}
}