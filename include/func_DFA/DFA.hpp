#ifndef func_DFA_HPP_
#define func_DFA_HPP_

#include <queue>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>


namespace func_DFA {

class DFA5 {
   private:
    // Поля пятикортежа
    std::string start_state;
    std::vector<std::string> states;
    std::vector<std::string> permitted_states;
    std::vector<std::string> alphabet;
    std::unordered_map<std::string, std::unordered_map<std::string, std::string>> transitions;

    // Дополнительные поля
    std::vector<std::string> string_transition;
    std::string current_state;

    // Приватный конструктор
    DFA5(const std::string& start, const std::vector<std::string>& st,
         const std::vector<std::string>& perm, const std::vector<std::string>& alph,
         const std::unordered_map<std::string, std::unordered_map<std::string, std::string>>& trans,

         const std::vector<std::string>& str_transition, const std::string current

    );

    DFA5 CreateNewDFAwithPartition(const std::vector<std::vector<std::string>>& Partition) const;

    std::vector<std::string> CreateNewStates(
        const std::vector<std::vector<std::string>>& Partition) const;
    std::string CreateStateName(const std::vector<std::string>& cls) const;
    std::unordered_map<std::string, std::string> CreateStateToNewState(
        const std::vector<std::vector<std::string>>& Partition) const;
    std::unordered_map<std::string, std::unordered_map<std::string, std::string>>
    CreateNewTransitions(const std::vector<std::vector<std::string>>& Partition,
                         const std::unordered_map<std::string, std::string>& stateToNewState) const;
    std::vector<std::string> CreateNewPermittedStates(
        const std::unordered_map<std::string, std::string>& stateToNewState) const;

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
    void exportStringTransitionCSV(std::ofstream& file);
    void exportPermittedStatesCSV(std::ofstream& file);
    void exportStartStateCSV(std::ofstream& file);
    void exportAlphabetCSV(std::ofstream& file);
    void exportTransitionsCSV(std::ofstream& file);

    bool stateIt();
    bool SymIt(std::string symbol);
    std::string transition(const std::string& state, const std::string& symbol);

   public:
    void print();
    void exportCSV(const std::string& filename);
    void minimize();
    void transitInput(const std::vector<std::string>& input);
    void transitFromCSV();
    void resetCurrentState();

    // Геттеры
    const std::string& getStartState() const;
    const std::vector<std::string>& getStates() const;
    const std::vector<std::string>& getPermittedStates() const;
    const std::vector<std::string>& getAlphabet() const;
    const std::unordered_map<std::string, std::unordered_map<std::string, std::string>>&
    getTransitions() const;
    const std::vector<std::string>& getStringTransition() const;
    const std::string& getCurrentState() const;

    friend class DFABuilder;
    friend class benchmark;
    friend class HopcroftMinimizer;

    friend class DFATestHelper;  // для тестов
};

class DFABuilder {
   private:
    // Поля пятикортежа
    std::string start_state;
    std::vector<std::string> states;
    std::vector<std::string> permitted_states;
    std::vector<std::string> alphabet;
    std::unordered_map<std::string, std::unordered_map<std::string, std::string>> transitions;

    // Дополнительные поля
    std::vector<std::string> string_transition;
    std::string current_state;

    // Используются в withCSV()
    void readStringTransition(std::ifstream& file);
    void readPermittedStates(std::ifstream& file);
    void readStartState(std::ifstream& file);
    void readAlphabet(std::ifstream& file);
    void readStatesAndTransitions(std::ifstream& file);

    // Используются в generatedDFA()
    void generateStates(int number_of_states);
    void generateAlphabet(int alphabet_size);
    void generateTransitions();
    int computeCapacity(const std::string& mode);
    std::vector<std::pair<std::string, std::string>> computeAvailableKeys(int seed);
    void addTransitionsToCapacity(std::vector<std::pair<std::string, std::string>> available_keys,
                                  int capacity, int seed);

   public:
    DFABuilder& withCSV(const std::string& filename);  // Чтение CSV
    DFABuilder& generatedDFA(int number_of_states, int alphabet_size, const std::string& mode,
                             int seed);  // генерация ДКА с заданными параметрами
    DFABuilder& withComponents(
        std::string start_state, std::vector<std::string> states,
        std::vector<std::string> permitted_states, std::vector<std::string> alphabet,
        std::unordered_map<std::string, std::unordered_map<std::string, std::string>>
            transitions);  // создание ДКА из компонент

    DFA5 build() const;  // Создание DFA
};

class HopcroftMinimizer {
   private:
    DFA5 dfa;  // копия автомата для минимизации

    // поля, используемые в процессе минимизации
    std::unordered_map<std::string, std::unordered_map<std::string, std::vector<std::string>>>
        invariant_transitions;
    std::vector<std::unordered_set<std::string>> Partition;
    std::unordered_map<std::string, int> StateToClass;
    std::queue<std::pair<int, std::string>> Queue;
    std::unordered_set<std::string> splitter;
    std::string symbol;
    int Class_id;
    std::vector<int> Count;
    std::vector<int> Twin;
    std::vector<int> Involved;

    // конструктор, для копирования ДКА в dfa
    explicit HopcroftMinimizer(const DFA5& input_dfa);

    // Основная функция
    std::vector<std::vector<std::string>> computePartition();

    // Вспомогательные функции для computePartition()
    void ValidateInput();

    void removeUnreachableStates();
    std::unordered_set<std::string> findReachableStates();
    void buildReachableStates(const std::unordered_set<std::string>& reachableStates,
                              std::vector<std::string>& newStates);
    void buildReachableTransitions(
        const std::unordered_set<std::string>& reachableStates,
        std::unordered_map<std::string, std::unordered_map<std::string, std::string>>&
            newTransitions);
    void buildReachablePermittedStates(const std::unordered_set<std::string>& reachableStates,
                                       std::vector<std::string>& newPermittedStates);

    void CreateInvariantTransitions();
    void InitPartition();
    void InitClass();
    void InitQueue();
    void takeSplitter();
    void fillInvolved();
    void split();
    void moveStatesInPartition();
    void moveStatesInClass();
    std::vector<std::vector<std::string>> getResultPartition();

    friend class DFA5;
    friend class DFATestHelper;
};

// =================================================================================

// Для тестов
class DFATestHelper {
   public:
    static std::vector<std::vector<std::string>> getPartition(const DFA5& dfa);
    static DFA5 getNewDFAwithPartition(const DFA5& dfa,
                                       const std::vector<std::vector<std::string>>& Partition);
};

// =================================================================================

class benchmark {
   private:
    std::vector<int> vector_alphabet_size;
    std::vector<int> vector_number_of_states;
    std::string mode;
    int repetitions;
    int seed;

    // создание массив размеров ДКА (number_of_states)
    int min_states;
    int max_states;
    int intermediate_number;

    // Приватный конструктор
    benchmark(std::vector<int> vector_alph_size, std::vector<int> vector_number_of_st,
              std::string mod, int repetit, int sed);

    void save_benchmark_data(const std::vector<std::pair<int, double>>& data,
                             const std::string& filename);
    void plot_with_gnuplot(const std::string& data_file,
                           const std::string& output_png = "data/dfa_min_time.png");

   public:
    void run_benchmark();
    void print();

    friend class benchmarkBuilder;
};

class benchmarkBuilder {
   private:
    std::vector<int> vector_alphabet_size;
    std::vector<int> vector_number_of_states;
    std::string mode;
    int repetitions;
    int seed;

   public:
    benchmarkBuilder& withInput(int min_states, int max_states, int intermediate_number, int seed,
                                int repetitions, std::string mode,
                                std::vector<int> vector_number_of_states,
                                std::vector<int> vector_alphabet_size);
    benchmark build() const;
};

}  // namespace func_DFA

#endif  // func_DFA_HPP_