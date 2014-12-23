#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <cstdlib>
#include <ctime>

#include "sort.hh"
#include "test.hh"
#include "timsort.hpp"


unsigned **gen_arrays(unsigned samples, unsigned size) {
    unsigned **arrays = new unsigned*[samples];

    srand(time(NULL));
    for ( unsigned i = 0; i < samples; ++i ) {
        unsigned *arr = new unsigned[size];

        for ( unsigned j = 0; j < size; ++j ) {
            arr[j] = rand() % (size * 10);
        }

        arrays[i] = arr;
    }

    return arrays;
}

std::vector<unsigned> *gen_vectors(unsigned samples, unsigned size) {
    std::vector<unsigned> *vectors = new std::vector<unsigned>[samples];

    srand(time(NULL));
    for ( unsigned i = 0; i < samples; ++i ) {
        std::vector<unsigned> vec(size);

        for ( unsigned j = 0; j < size; ++j ) {
            vec[j] = rand() % (size * 10);
        }

        vectors[i] = vec;
    }

    return vectors;
}

int main(int argc, char **argv) {
    std::map<char *, array_fp> const sorts = {
        {"std::sort", &std::sort<unsigned *>},
        {"std::stable_sort", &std::stable_sort<unsigned *>},
        // {"sort::insertion_sort", &sort::insertion_sort<unsigned *>},
        // {"sort::shellsort", &sort::shellsort<unsigned *>},
        // {"sort::smoothsort", &sort::smoothsort<unsigned *>},
        // {"sort::quicksort", &sort::quicksort<unsigned *>},
        // {"sort::introsort", &sort::introsort<unsigned *>},
        // {"sort::parallel_quicksort", &sort::parallel_quicksort<unsigned *>},
        // {"sort::parallel_introsort", &sort::parallel_introsort<unsigned *>},
        // {"gfx::timsort", &gfx::timsort}
    };
    
    char *name;
    array_fp fp;

    for ( std::map<char *, array_fp>::const_iterator it = sorts.begin(); it != sorts.end(); ++it ) {
        name = it->first;
        fp = it->second;
        std::cout << name << std::endl;

        for ( unsigned i = 100; i <= SIZE; i += 100 ) {
            std::pair<int, double> res = 
                benchmark<unsigned>(SAMPLES, i, fp);
            std::cout << i << ' ' << res.first << ' ' << res.second << std::endl;
        }
    }

    return 0;
}