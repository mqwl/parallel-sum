# parallel-sum
Parallel summation using different algorithms, written in C and C++.

### Structure 
`main` - benchmarking algorithms and writing results into .csv

`sums` - implementation of algorithms using OpenMP:
* sum_seq - Sequential sum (single thread)
* sum_ar - Auto reduction
* sum_rr - Round-Robin
* sum_rr_fs - Round-Robin with false sharing mitigation by spacing out thread results
* sum_rr_fs2 - Round-Robin with false sharing mitigation by accumulating thread results into private register

`loc` - implementation of algorithms using mutex:
* sum_loc
* sum_loc_reduce

`barrier` - implementation of parallel reduction using barriers

`visualize` - Python script that builds graphs for time, speedup and efficiency of every algorithm (requires `pandas` and `matplotlib`)
