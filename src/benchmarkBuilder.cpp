#include <func_DFA/DFA.hpp>

namespace func_DFA {
using std::string;
using std::vector;

benchmarkBuilder& benchmarkBuilder::withInput(int min_states, int max_states,
                                              int intermediate_number, int seed, int repetitions,
                                              string mode, vector<int> vector_number_of_states,
                                              vector<int> vector_alphabet_size) {
    // Сохраняем параметры в поля

    this->seed = seed;
    this->repetitions = repetitions;
    this->mode = mode;
    this->vector_alphabet_size = vector_alphabet_size;

    // массив размеров ДКА
    int n = min_states;
    // увеличение размеров ДКА в зависимости n**2
    while (n < max_states) {
        if (n == 1) {
            n = 2;
        }  // если начали с 1, следующая степень 2
        else
            n *= 2;

        // промежуточные значения
        if (!vector_number_of_states.empty()) {
            ;
            int last = vector_number_of_states.back();
            int step = (n - last) / intermediate_number;
            if (vector_number_of_states.back() + step * (intermediate_number - 1) < n && step > 0) {
                for (int count = 0; count < intermediate_number - 1; ++count) {
                    vector_number_of_states.push_back(vector_number_of_states.back() + step);
                }
            }
        }
        vector_number_of_states.push_back(n);
    }
    this->vector_number_of_states = vector_number_of_states;
    return *this;
};

benchmark benchmarkBuilder::build() const {
    return benchmark(vector_alphabet_size, vector_number_of_states, mode, repetitions, seed);
}

}  // namespace func_DFA
