set terminal png size 800,600
set output 'data/dfa_min_time.png'
set xlabel 'Number of states'
set ylabel 'Execution time (ms)'
set title 'DFA minimization performance'
set grid
plot 'data/dfa_benchmark_data.txt' using 1:2 with linespoints title 'DFAmin'
