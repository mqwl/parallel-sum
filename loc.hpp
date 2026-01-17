#pragma once

#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <chrono>


unsigned sum_loc(const unsigned*, size_t);
unsigned sum_loc_reduce(const unsigned*, size_t);