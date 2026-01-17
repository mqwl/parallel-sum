#include <stdio.h>
#include <string.h>
#include <omp.h>
#include <stdlib.h>
#include "sums.h"

unsigned sum_seq(const unsigned* V, size_t n) {
    unsigned res = 0;
    for (size_t i = 0; i < n; ++i) {
        res = res + V[i];
    }
    return res;
}
unsigned sum_ar(const unsigned* V, size_t n) {
    unsigned res = 0;
    #pragma omp parallel for reduction (+:res)
    for (size_t i = 0; i < n; ++i) {
        res = res + V[i];
    }
    return res;
    
}
unsigned sum_rr(const unsigned* V, size_t n) {
    unsigned s = 0;
    unsigned p = omp_get_num_procs();
    unsigned* partial_sums = (unsigned*) calloc(p, sizeof(unsigned));
    #pragma omp parallel
    {
        unsigned T = omp_get_num_threads(), t = omp_get_thread_num();
        for (size_t i = t; i < n; i += T) {
            partial_sums[t] += V[i];
        }
    }
    for (size_t i = 0; i < p; ++i) {
        s += partial_sums[i];
    }
    free(partial_sums);
    return s;
}

unsigned sum_rr_fs(const unsigned* V, size_t n) {
    unsigned *partial, T;
    #pragma omp parallel shared(T)
    {
        size_t t = omp_get_thread_num();
        #pragma omp single 
        {
            T = omp_get_num_threads();
            partial = (unsigned*) calloc(CACHE_SIZE, T);
        }
        for (size_t i = t; i < n; i += T) {
            partial[(t * CACHE_SIZE) / sizeof(unsigned)] += V[i];
        }
    }
    unsigned s = 0;
    for (size_t t = 0; t < T; ++t) {
        s += partial[(t * CACHE_SIZE) / sizeof(unsigned)];
    }
    free(partial);
    return s;
}

unsigned sum_rr_fs2(const unsigned* V, size_t n) {
    struct partial_t* partial;
    unsigned T;
    #pragma omp parallel shared(partial)
    {
        unsigned t = omp_get_thread_num();
        #pragma omp single 
        {
            T = omp_get_num_threads();
            partial = (struct partial_t*) calloc(T, sizeof(struct partial_t));
        }
        unsigned nn = 0;
        for (size_t i = t; i < n; i += T) {
            nn += V[i];
        }
        partial[t].v = nn;
    }
    unsigned s = 0;
    for (size_t t = 0; t < T; ++t) {
        s += partial[t].v;
    }
    free(partial);
    return s;
}