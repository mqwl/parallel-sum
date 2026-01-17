#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <chrono>
#include <condition_variable>
#include "barrier.hpp"
#include "conf.h"


Latch::Latch(unsigned ps): pool_size(ps) {}

void Latch::arrive_and_wait() {
    std::unique_lock l{m};
    if (++counter < pool_size) do
        cv.wait(l); while (counter < pool_size);
    else
        cv.notify_all();
}

Barrier::Barrier(unsigned ps): pool_size(ps) {}

void Barrier::arrive_and_wait() {
    std::unique_lock l{m};
    if (++counter < pool_size) {
        bool my_barrier = current_instance;
        do {
            cv.wait(l); 
        } while (my_barrier == current_instance);
    }
    else {
        counter = 0;
        current_instance = !current_instance;
        cv.notify_all();
    }
}

void Semaphore::acquire() {
    std::unique_lock l{m};
    passed++;
    while (passed >= max_threads) {
        cv.wait(l);
    }
}

void Semaphore::release() {
    std::unique_lock l{m};
    passed--;
    cv.notify_one();
}

std::pair<size_t, size_t> get_subvector(size_t n, unsigned t, unsigned T) {
    size_t s, b, e;
    if (t < n % T)
        s = n / T + 1;
    else
    s = n / T;
    if (t <= n % T)
        b = t * (n / T + 1);
    else
        b = t * (n / T) + (n % T);
    e = b + s;
    return {b, e};
}

// ПАРАЛЛЕЛЬНАЯ РЕДУКЦИЯ
unsigned sum_reduce_cpp (const unsigned* V, size_t n) {
    unsigned T = get_num_threads();
    std::vector<std::thread> workers(T - 1);
    Barrier bar{T};
    auto partial_sums = std::make_unique<partial_t[]> (T);
    auto worker_proc = [V, n, T, &bar, &partial_sums] (unsigned t) {
        auto [b, e] = get_subvector(n, t, T);
        unsigned r = 0;
        for (size_t i = b; i < e; ++i) {
            r += V[i];
        }
        partial_sums[t].u = r;
        // Редукция
        // 000 /001/ 010 /011/ 100 /101/ 110
        //   ^    ^    ^    ^    ^    ^    ^
        // ^ - rem
        // step = 2 => rem = 2; step = 4 = > rem = 8;
        for (size_t step = 1, rem = 2; step < T; step = rem, rem *= 2) {
            bar.arrive_and_wait();
            if (t % rem == 0 && t + step < T) {
                partial_sums[t].u += partial_sums[t + step].u;
            }
        }
    };
    for (unsigned t = 0; t < T - 1; ++t) {
        workers[t] = std::thread(worker_proc, t + 1);
    }
    worker_proc(0);
    for (unsigned i = 0; i < T - 1; ++i) {
        workers[i].join();
    }
    return partial_sums[0].u;
}
