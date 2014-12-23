#include "bitonic.hh"
#include "bitonic_kernel.cuh"


#define SAMPLES 10
#define SIZE 100000
#define STEP 100

int main () {
    for ( int i = 128; i <= SIZE; i <<= 1 ) {
        std::pair<int, double> res = 
            benchmark<unsigned>(SAMPLES, i, &bitonic_sort<unsigned>);
        std::cout << i << ' ' << res.first << ' ' << res.second << std::endl;
    }

    return 0;
}