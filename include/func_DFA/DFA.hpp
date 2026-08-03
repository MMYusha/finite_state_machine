#ifndef func_DFA_HPP_
#define func_DFA_HPP_

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <queue>


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

        //vector<vector<string>> computePartition() const;
        DFA5 CreateNewTransitions(const vector<vector<string>>& Partition) const;
        void transit_string();

        // Используются в print()
        void printStartState();
        void printStates();
        void printPermittedStates();
        void printAlphabet();
        void printTransitions();
        void printStringTransition();
        void printCurrentState();

        // Используются в exportCSV()
        void exportStringTransitionCSV(ofstream& file);
        void exportPermittedStatesCSV(ofstream& file);
        void exportStartStateCSV(ofstream& file);
        void exportAlphabetCSV(ofstream& file);
        void exportTransitionsCSV(ofstream& file);

        bool stateIt();
        bool SymIt(string symbol);
        string transition(const string& state, const string& symbol);

    public:
        void print();
        void exportCSV(const string& filename);
        void minimize();
        void transitInput(const vector<string>& input);
        void transitFromCSV();
        void resetCurrentState();

        //Геттеры
        const string& getStartState() const;   
        const vector<string>& getStates() const; 
        const vector<string>& getPermittedStates() const;
        const vector<string>& getAlphabet() const;
        const unordered_map<string, unordered_map<string, string>>& getTransitions() const;
        const vector<string>& getStringTransition() const;
        const string& getCurrentState() const;

        friend class DFABuilder;
        friend class benchmark;
        friend class HopcroftMinimizer;

        friend class DFATestHelper; // для тестов
        
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

        // Используются в withCSV()
        void readStringTransition(ifstream& file);
        void readPermittedStates(ifstream& file);
        void readStartState(ifstream& file);
        void readAlphabet(ifstream& file);
        void readStatesAndTransitions(ifstream& file);
        
        // Используются в generatedDFA()
        void generateStates(int number_of_states);
        void generateAlphabet(int alphabet_size);
        void generateTransitions();
        int computeCapacity(const string& mode);
        vector<pair<string, string>> computeAvailableKeys(int seed);
        void addTransitionsToCapacity(vector<pair<string, string>> available_keys, int capacity, int seed);

    public:
        DFABuilder& withCSV(const std::string& filename); // Чтение CSV
        DFABuilder& generatedDFA(int number_of_states, int alphabet_size, const string& mode, int seed); // генерация ДКА с заданными параметрами
        DFABuilder& withComponents( string start_state, 
                                    vector<string> states, 
                                    vector<string> permitted_states, 
                                    vector<string> alphabet, 
                                    unordered_map<string, unordered_map<string, string>> transitions); // создание ДКА из компонент

        DFA5 build() const; // Создание DFA
};

class HopcroftMinimizer {
    private:
        DFA5 dfa;  // копия автомата для минимизации

        // поля, используемые в процессе минимизации
        unordered_map<string, unordered_map<string, vector<string>>> invariant_transitions;
        vector<unordered_set<string>> Partition;
        unordered_map<string, int> StateToClass;
        queue<pair<int,string>> Queue;
        unordered_set<std::string> splitter;
        string symbol;
        int Class_id;
        vector<int> Count;
        vector<int> Twin;
        vector<int> Involved;


        // конструктор, для копирования ДКА в dfa
        explicit HopcroftMinimizer(const DFA5& input_dfa);


        // Основная функция
        vector<vector<string>> computePartition();

        // Вспомогательные функции для computePartition()
        void ValidateInput();
        void removeUnreachableStates();
        void CreateInvariantTransitions();
        void InitPartition();
        void InitClass();
        void InitQueue();
        void takeSplitter();
        void fillInvolved();
        void split();
        void moveStatesInPartition();
        void moveStatesInClass();
        vector<vector<string>> getResultPartition();



    
    friend class DFA5;
    friend class DFATestHelper;

};

// =================================================================================

// Для тестов
class DFATestHelper {
    public:
        static vector<vector<string>> getPartition(const DFA5& dfa);
        static DFA5 getNewDFAwithPartition(const DFA5& dfa, const vector<vector<string>>& Partition);
    };

// =================================================================================

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