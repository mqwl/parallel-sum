#include <thread>
#include <vector>
#include <mutex>
#include "loc.hpp"

unsigned sum_loc (const unsigned* V, size_t n) {
    unsigned T = std::thread::hardware_concurrency();
    std::vector<std::thread> workers(T - 1);
    unsigned s = 0;
    std::mutex mtx;
    auto worker_proc = ([V, n, T, &s, &mtx](unsigned t) {
        size_t a = n % T, b = n / T;
        if (t < b) {
            a = t * ++b;
        } else {
            a += t * b;
        }
        b += a;
        unsigned res = 0;
        for (size_t i = a; i < b; i++) {
            res += V[i];
        }
        s += res;
    });
    {
        for (unsigned t = 0; t < T - 1; ++t) {
            workers[t] = std::thread(worker_proc, t + 1);
        }
        worker_proc(0);
        for (unsigned t = 0; t < T - 1; ++t) {
            workers[t].join();
        }
    }
    return s;
}

unsigned sum_loc_reduce (const unsigned* V, size_t n) {
    unsigned T = std::thread::hardware_concurrency();
    std::vector<std::thread> workers(T - 1);
    unsigned partial_sums[T];
    unsigned s = 0;
    std::mutex mtx;
    auto worker_proc = ([V, n, T, &mtx, &partial_sums](unsigned t) {
        size_t a = n % T, b = n / T;
        if (t < b) {
            a = t * ++b;
        } else {
            a += t * b;
        }
        b += a;
        unsigned res = 0;
        for (size_t i = a; i < b; i++) {
            res += V[i];
        }
        partial_sums[t] = res;
    });
    {
        for (unsigned t = 0; t < T - 1; ++t) {
            workers[t] = std::thread(worker_proc, t + 1);
        }
        worker_proc(0);
        for (unsigned t = 0; t < T - 1; ++t) {
            workers[t].join();
        }
        for (size_t i = 0; i < T; ++i) {
            s += partial_sums[i];
        }
    }
    return s;
}
