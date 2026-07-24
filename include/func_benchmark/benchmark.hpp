#ifndef func_benchmark_DFAmin_HPP_
#define func_benchmark_DFAmin_HPP_

#include <iostream>   // для std::cout
#include <vector>
#include <string>

// Объявление функции бенчмарка

namespace func_benchmark {
    void run_benchmark(
        std::vector<int> number_of_states,
        int alphabet_size = 2,
        std::string = "full",
        int repetitions = 3,
        int seed = 42
    );
}
#endif // func_benchmark_DFAmin_HPP_