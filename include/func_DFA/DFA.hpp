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
        DFA5 build() const; // Создание DFA
};


struct DFA {
    vector<string> string_transition;
    vector<string> permited_state;
    string start_state;
    vector<string> alphabet;
    vector<string> states;
    unordered_map<string, unordered_map<string, string>> transitions;
};

string transit(const vector<string>& input, string current,
              const unordered_map<string, unordered_map<string, string>>& transitions);

vector<vector<string>> DFAmin(
    const vector<string>& alphabet,
    const vector<string>& Q, 
    const vector<string>& F,
    const unordered_map<string, unordered_map<string, string>>& transitions);

DFA CreateNewTransitions(
    DFA OldDfa,
    vector<vector<string>> P);

DFA DFAinput(string filename);
void writeDFA(const string& filename, const DFA& dfa);

void run_benchmark(
        vector<int> number_of_states,
        vector<int> vector_alphabet_size,
        string = "full",
        int repetitions = 3,
        int seed = 42
    );


} // namespace func_transition

#endif // func_DFA_HPP_