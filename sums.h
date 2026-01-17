#pragma once

#include <vector>
#include <iomanip>
#include <iostream>
#include <chrono>
#include <omp.h>
#include "conf.h"
#define CACHE_SIZE 64 // в <thread> есть std::hardware_destructive_interference_size

struct partial_t {
    alignas(CACHE_SIZE) unsigned v;
};

unsigned sum_seq(const unsigned*, size_t);
unsigned sum_ar(const unsigned*, size_t);
unsigned sum_rr(const unsigned*, size_t);
unsigned sum_rr_fs(const unsigned*, size_t);
unsigned sum_rr_fs2(const unsigned*, size_t);
