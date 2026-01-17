#pragma once

#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <chrono>

// Люк - одноразовый барьер
class Latch {
    public:
        Latch (unsigned pool_size);
        void arrive_and_wait();
    private:
        unsigned counter = 0;
        const unsigned pool_size;
        std::mutex m;
        std::condition_variable cv;
};

// Барьер блокирует группу потоков размера pool_size, пока все они не дойдут до барьера
// т.е пока одни ещё доходят до барьера, уже дошедшие заблокированы
class Barrier {
    public:
        Barrier (unsigned pool_size);
        void arrive_and_wait();
    private:
        bool current_instance = true;
        unsigned counter = 0;
        const unsigned pool_size;
        std::mutex m;
        std::condition_variable cv;
};



// ДРОССЕЛИРОВАНИЕ СЕМАФОРАМИ
// Семафор открыт, пока к нему подходит не более n потоков
class Semaphore {
    public:
        Semaphore (unsigned max_thr) : max_threads(max_thr) {}
        void acquire();
        void release();
    private:
        unsigned const max_threads;
        unsigned passed = 0;
        std::mutex m;
        std::condition_variable cv;
};

std::pair<size_t, size_t> get_subvector(size_t n, unsigned t, unsigned T);

unsigned sum_reduce_cpp(const unsigned*, size_t);
