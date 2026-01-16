#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <chrono>
#include "loc.hpp"
union partial_t {

};

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

// using namespace std::chrono;

// auto generate(size_t n) {
//     std::vector<unsigned> v(n);
//     for (size_t i = 0; i < n; i++) {
//         v[i] = i;
//     }
//     return v;
// }
// int main() {
//     constexpr size_t N = 1 << 22;
//     auto input = generate(N);
//     {
//         auto t1 = steady_clock::now();
//         auto s1 = sum_loc(input.data(), input.size());
//         auto t2 = steady_clock::now();
//         std::cout << "\nsum_loc: " << std::hex << s1 << ", time: " << std::dec << duration_cast<milliseconds>(t2-t1).count();
//         t1 = steady_clock::now();
//         s1 = sum_loc_reduce(input.data(), input.size());
//         t2 = steady_clock::now();
//         std::cout << "\nsum_loc_reduce: " << std::hex << s1 << ", time: " << std::dec << duration_cast<milliseconds>(t2-t1).count();
//     }
//     return 0;
// }