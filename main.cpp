#include <omp.h>
#include <chrono>
#include <iostream>
#include <vector>
#include <thread>
#include <iomanip>
#include <condition_variable>
#include "loc.hpp"
#include "sums.h"
#include "barrier.hpp"
#include "conf.h"


#define SUM_COMMAND(NAME)   { #NAME, sum_ ## NAME }

typedef unsigned (*sumfunc)(const unsigned *, size_t);

struct sum_command {
	const char *name;
	sumfunc func;
};

struct exp_result {
	double time, speedUp, efficiency;
	unsigned T, result;
};

auto generate (size_t n) {
	std::vector<unsigned> v(n);
	for (size_t i = 0; i < n; ++i)
		v[i] = i;
	return v;
}

std::vector<exp_result> conduct_experiment(sumfunc f, const size_t problem_size) {
	using namespace std::chrono;
	auto v = generate(problem_size);
	unsigned T = get_num_threads();
	std::vector<exp_result> results(T);
	set_num_threads(1);
	auto t0 = steady_clock::now();
	auto base_result = f(v.data(), v.size());
	auto t1 = steady_clock::now();
	double base_time = duration_cast<milliseconds>(t1 - t0).count();
	results[0] = {
		.time = base_time,
		.speedUp = 1.0,
		.efficiency = 1.0,
		.T = (unsigned )1,
		.result = base_result,
	};
	for (unsigned threads = 2; threads <= T; ++threads) {
		set_num_threads(threads);
		v = generate(problem_size);
		t0 = steady_clock::now();
		auto result = f(v.data(), v.size());
		t1 = steady_clock::now();
		double time = duration_cast<milliseconds> (t1 - t0).count();
		double speedUp = base_time / time;
		double efficiency = speedUp / threads;
		results[threads-1] = {
			.time = time,
			.speedUp = speedUp,
			.efficiency = efficiency,
			.T = (unsigned) threads,
			.result = result,
		};
	}
	return results;
}

void print_csv_results (const std::vector<exp_result> &results, const char* name) {
	for (const auto &r : results) {
		std::cout << name << ","
			<< r.T << ","
			<< r.time << ","
			<< r.speedUp << ","
			<< r.efficiency << ",0x" <<
			std::hex << r.result << "\n";
	}	
}

int main() {
	constexpr size_t N = 1 << 22;
	struct sum_command const sum_commands[] = {
		SUM_COMMAND (seq),
		SUM_COMMAND (ar),
		SUM_COMMAND (rr),
		SUM_COMMAND (rr_fs),
		SUM_COMMAND (rr_fs2),
		SUM_COMMAND (loc),
		SUM_COMMAND (loc_reduce),
		SUM_COMMAND (reduce_cpp)
	};

	std::cout << "Algorithm,Nthreads,Time(ms),SpeedUp,Efficiency,Result\n";
	for (size_t cmdi = 0; cmdi <
			sizeof (sum_commands) / sizeof (struct sum_command); ++cmdi)
	{
		auto results = conduct_experiment(sum_commands[cmdi].func, N);
		print_csv_results(results, sum_commands[cmdi].name);
	}
}

