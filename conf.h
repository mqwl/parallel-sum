#pragma once

#define CACHE_SIZE 64 // в <thread> есть std::hardware_destructive_interference_size

extern "C" {
    unsigned get_num_threads ();
    unsigned set_num_threads (unsigned);
}

struct partial_t {
    alignas(CACHE_SIZE) unsigned u;
};