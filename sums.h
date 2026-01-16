#pragma once

#include <vector>
#include <iomanip>
#include <iostream>
#include <chrono>
#include <omp.h>
#include "conf.h"

unsigned sum_seq(const unsigned*, size_t);
unsigned sum_ar(const unsigned*, size_t);
unsigned sum_rr(const unsigned*, size_t);
unsigned sum_rr_fs(const unsigned*, size_t);
unsigned sum_rr_fs2(const unsigned*, size_t);
