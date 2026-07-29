#ifndef func_DFA_HPP_
#define func_DFA_HPP_

#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

namespace func_DFA {

class DFA5 {
    private:
        // Поля пятикортежа 
        string start_state;   
        vector<string> states; 
        vector<string> permitted_states;
        vector<string> alphabet;
        unordered_map<string, unordered_map<string, string>> transitions;

        // Дополнительные поля
        vector<string> string_transition;
        string current_state;


        // Приватный конструктор
        DFA5(
            const string& start,
            const vector<string>& st,
            const vector<string>& perm,
            const vector<string>& alph,
            const unordered_map<string, unordered_map<string, string>>& trans,

            const vector<string>& str_transition,
            const string current
            
        );

        vector<vector<string>> computePartition() const;
        DFA5 CreateNewTransitions(const vector<vector<string>>& Partition) const;
        void transit_string(const vector<string>& str);

    public:
        //static DFA5 fromCSV(const string& filename);
        void print();
        void exportCSV(const string& filename);
        void minimize();
        void transit_Input(const vector<string>& input);
        void transit_fromCSV();

        friend class DFABuilder;
        friend class benchmark;
};

class DFABuilder {
    private:
        // Поля пятикортежа 
        string start_state;   
        vector<string> states; 
        vector<string> permitted_states;
        vector<string> alphabet;
        unordered_map<string, unordered_map<string, string>> transitions;

        // Дополнительные поля
        vector<string> string_transition;
        string current_state;

    public:
        DFABuilder& withCSV(const std::string& filename); // Чтение CSV
        DFABuilder& generatedDFA(int number_of_states, int alphabet_size, const string& mode, int seed); // генерация ДКА с заданными параметрами
        DFA5 build() const; // Создание DFA
};


class benchmark{
    private:
        vector<int> vector_alphabet_size;
        vector<int> vector_number_of_states;
        string mode;
        int repetitions;
        int seed;

        // создание массив размеров ДКА (number_of_states)
        int min_states;
        int max_states;
        int intermediate_number;

        // Приватный конструктор
        benchmark(
            vector<int> vector_alph_size,
            vector<int> vector_number_of_st,
            string mod,
            int repetit,
            int sed
        );

        void save_benchmark_data(const vector<pair<int, double>>& data, const string& filename);
        void plot_with_gnuplot(const string& data_file, const string& output_png = "dfa_min_time.png"); 


    public:

        void run_benchmark();
        void print();

        friend class benchmarkBuilder;
};


class benchmarkBuilder {
    private:
        vector<int> vector_alphabet_size;
        vector<int> vector_number_of_states;
        string mode;
        int repetitions;
        int seed;

    public:
        benchmarkBuilder& withInput(int min_states, int max_states, int intermediate_number,
            int seed, int repetitions, string mode,  vector<int> vector_number_of_states, vector<int> vector_alphabet_size);
        benchmark build() const;
};

} // namespace func_transition

#endif // func_DFA_HPP_