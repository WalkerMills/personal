#ifndef __TEST_HH__
#define __TEST_HH__

#include <chrono>
#include <numeric>
#include <utility>
#include <vector>
#include <cmath>

// #include <mpi.h>

#include "sort.hh"

// Default number of samples for benchmarking 
#define SAMPLES 10
#define SIZE 100000


typedef void (*array_fp)(unsigned *start, unsigned *stop);
typedef void (*vector_fp)(std::vector<unsigned>::iterator start,
                          std::vector<unsigned>::iterator stop);

unsigned **gen_arrays(unsigned samples, unsigned size);
std::vector<unsigned> *gen_vectors(unsigned samples, unsigned size);

void benchmark_array(unsigned samples, unsigned size, array_fp f);
void benchmark_vector(unsigned samples, unsigned size, vector_fp f);

template<typename T>
double mean(T start, T stop) {
    double sum = 0;
    unsigned count = 0;

    while ( start < stop ) {
        sum += *start++;
        ++count;
    }

    return count ? sum / count : count;
}

template<typename T>
double stddev(T start, T stop, double mu) {
    double sum = 0;
    unsigned count = 0;

    while ( start < stop ) {
        sum += pow(*start++ - mu, 2);
        ++count;
    }

    return count ? sqrt(sum / count) : count;
}

template<typename T>
double stddev(T start, T stop) {
    double mu = mean(start, stop);

    return stddev(start, stop, mu);
}

// Benchmark a sorting algorithm and check it for correctness
template<typename T, typename F>
std::pair<int, double> benchmark(unsigned samples, unsigned size, F fp) {
    T **arrays = gen_arrays(samples, size);
    double *res = new double[samples];

    std::chrono::microseconds delta;
    std::chrono::high_resolution_clock::time_point start, end;

    // Benchmark the sorting algorithm
    for ( unsigned i = 0; i < samples; ++i ) {
        start = std::chrono::high_resolution_clock::now();
        fp(arrays[i], arrays[i] + size);
        end = std::chrono::high_resolution_clock::now();
        delta = 
            std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        res[i] = delta.count();
    }

    for ( unsigned i = 0; i < samples; ++i ) {
        if ( ! sort::check(arrays[i], arrays[i] + size) ) {
            std::cerr << "Warning: sample " << i << " is not sorted" 
                      << std::endl;
        }

        delete [] arrays[i];
    }
    delete [] arrays;    

    double mu = mean(res, res + samples);
    double sigma = stddev(res, res + samples, mu);

    delete [] res;

    return std::make_pair(mu, sigma);
}

#endif