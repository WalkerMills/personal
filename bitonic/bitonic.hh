#ifndef __BITONIC_HH__
#define __BITONIC_HH__

#include <chrono>
#include <iostream>
#include <sstream>
#include <utility>
#include <cmath>
#include <cstdlib>


unsigned **gen_arrays(int samples, int size) {
    unsigned **arrays = new unsigned*[samples];

    srand(time(NULL));
    for ( int i = 0; i < samples; ++i ) {
        unsigned *arr = new unsigned[size];

        for ( int j = 0; j < size; ++j ) {
            arr[j] = rand() % (size * 10);
        }

        arrays[i] = arr;
    }

    return arrays;
}


template<typename T>
int compare(const void *a, const void *b) {
    return *((T *) a) - *((T *) b);
}

// Return true iff the elements in the range [start, stop) are sorted
template<typename T>
bool check(T start, T stop) {
    for ( T i = start + 1; i < stop; ++i ) {
        if ( *(i - 1) > *i ) {
            return false;
        }
    }

    return true;
}

// Return true iff all the elements in the container are sorted
template<typename T>
bool check(T input) {
    return check(input.begin(), input.end());
}

// Print the elements in the range [first, last) 
template<typename T>
void print(T first, T last) {
    for ( T i = first; i < last; ++i ) {
        std::cout << *i << ' ';
    }
    std::cout << std::endl;
}

// Print all the elements in the container
template<typename T>
void print(T input) {
    print(input.begin(), input.end());
}

// If the elements in the range [start, stop) aren't sorted, then print
// the array, with brackets around runs in reverse sorted order, and
// return false. Otherwise, return true.
template<typename T>
bool show(T start, T stop) {
    bool sorted = true;
    std::stringstream ss;

    for ( T i = start; i < stop; ++i ) {
        if ( i < stop - 1 && *i > *(i + 1) ) {
            // There are unsorted elements in this range
            sorted = false;

            // Append the run of elements in reverse sorted order to the
            // stream
            ss << "[ " << *i++ << ' ';

            // Append while still reverse sorted
            for ( ; i < stop - 1 && *i > *(i + 1); ++i ) {
                std::cerr << *i << ' ';
            }

            // Finish appending the run
            ss << *i << " ] ";
        } else {
            // Append other elements to the stream
            ss << *i << ' ';
        }
    }

    // Print the range, if it's not sorted
    if ( ! sorted ) {
        std::cerr << ss.str() << std::endl;
    }

    return sorted;
}

// If the elements in the container aren't sorted, then print the array,
// with brackets around runs in reverse sorted order, and  return false.
// Otherwise, return true.
template<typename T>
bool show(T input) {
    return show(input.begin(), input.end());
}


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
std::pair<int, double> benchmark(int samples, int size, F fp) {
    T **arrays = gen_arrays(samples, size);
    double *res = new double[samples];
    int i;

    std::chrono::microseconds delta;
    std::chrono::high_resolution_clock::time_point start, end;

    // Benchmark the sorting algorithm
    for ( i = 0; i < samples; ++i ) {
        start = std::chrono::high_resolution_clock::now();
        fp(arrays[i], size, &compare<T>);
        end = std::chrono::high_resolution_clock::now();
        delta = 
            std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        res[i] = delta.count();

        if ( ! check(arrays[i], arrays[i] + size) ) {
            std::cerr << "Warning: sample " << i << " is not sorted" 
                      << std::endl;
        }
        delete [] arrays[i];
    }

    double mu = mean(res, res + samples);
    double sigma = stddev(res, res + samples, mu);

    delete [] res;

    return std::make_pair(mu, sigma);
}

#endif // __BITONIC_HH__
