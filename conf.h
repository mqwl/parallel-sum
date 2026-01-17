#pragma once

extern "C" {

unsigned get_num_threads ();

unsigned set_num_threads (unsigned);

}
#define CACHE_SIZE 64 // в <thread> есть std::hardware_destructive_interference_size

struct partial_t {
    alignas(CACHE_SIZE) unsigned u;
};