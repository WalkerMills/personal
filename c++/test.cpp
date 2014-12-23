#include <iostream>
#include <fstream>
#include <map>
#include <random>
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

float **gen_arrays(unsigned samples, unsigned size) {
    float **arrays = new float*[samples];

    srand(time(NULL));
    for ( unsigned i = 0; i < samples; ++i ) {
        float *arr = new float[size];

        for ( unsigned j = 0; j < size; ++j ) {
            arr[j] = (float)( ((double) rand() / (double) RAND_MAX) * 
                              ((double) rand() / (double) RAND_MAX) );
        }

        arrays[i] = arr;
    }

    return arrays;
}

double **gen_arrays(unsigned samples, unsigned size) {
    double **arrays = new double*[samples];
    std::default_random_engine gen;
    std::uniform_real_distribution<double> dist(0.0, 1.0);

    for ( unsigned i = 0; i < samples; ++i ) {
        double *arr = new double[size];

        for ( unsigned j = 0; j < size; ++j ) {
            arr[j] = dist(gen);
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
        {"std::sort", &std::sort},
        {"std::stable_sort", &std::stable_sort},
        // {"sort::insertion_sort", &sort::insertion_sort},
        // {"sort::shellsort", &sort::shellsort},
        {"sort::smoothsort", &sort::smoothsort},
        {"sort::quicksort", &sort::quicksort},
        {"sort::introsort", &sort::introsort},
        {"sort::parallel_quicksort", &sort::parallel_quicksort},
        {"sort::parallel_introsort", &sort::parallel_introsort},
        // {"gfx::timsort", &gfx::timsort}
    };

    for ( std::map<char *, array_fp>::const_iterator it = sorts.begin();
          it != sorts.end(); ++it ) {
        std::ofstream ofs(it->first);
        std::cout << it->first << ':' << std::endl;

        for ( unsigned i = 10; i <= SIZE; i *= 10 ) {
            unsigned samples = SIZE / i;

            std::pair<double, double> res = benchmark<float>(samples, i, 
                                                             it->second);

            ofs << i << ", " << res.first << ", " res.second << std::endl; 
        }

        ofs.close();
        // std::cout << res.first << " elements per microsecond, standard "
        //              "deviation of " << res.second 
        //           << " (" << 100 * res.second / res.first << "%) for "
        //           << SAMPLES << " samples of " << SIZE << " elements"
        //           << std::endl << std::endl;
    }

    return 0;
}
