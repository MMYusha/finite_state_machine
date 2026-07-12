#ifndef func_benchmark_DFAmin_HPP_
#define func_benchmark_DFAmin_HPP_

#include <iostream>   // для std::cout
#include <string>
// Объявление функции бенчмарка

namespace func_benchmark {
    void run_benchmark(
        int min_states = 10,
        int max_states = 200,
        int step = 10,
        int alphabet_size = 2,
        std::string = "full",
        int repetitions = 3,
        int seed = 42
    );
}
#endif // func_benchmark_DFAmin_HPP_