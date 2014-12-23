#ifndef __BITONIC_KERNEL_CU__
#define __BITONIC_KERNEL_CU__

#define CHECK_SYNC
#include "CudaErrorCheck.cu"
#include "bitonic_kernel.cuh"


template<typename T>
__global__ void bitonic_kernel(T *d_base, size_t num, int j, int k, 
                               int (*cmp)(const void *, const void *)) {
    int i, pair;
    T tmp, *i_ptr, *pair_ptr;

    i = threadIdx.x + blockIdx.x * blockDim.x;
    pair = i ^ j;
    if ( i >= num || i >= pair ) return;

    i_ptr = d_base + i;
    pair_ptr = d_base + pair;

    if ( ((i & k) == 0 && cmp(i_ptr, pair_ptr) > 0) ||
         ((i & k) != 0 && cmp(i_ptr, pair_ptr) < 0)) {
        tmp = *i_ptr;
        *i_ptr = *pair_ptr;
        *pair_ptr = tmp;
    }
}

template<typename T>
T maximum(T *base, size_t num) {
    T tmp, max = *base;

    while ( --num ) {
        tmp = *++base;
        if ( tmp > max ) max = tmp;
    }

    return max;
}

// Least power of 2 greater than or equal to x (from Hacker's Delight)
inline size_t clp2(size_t x) {
   x -= 1;
   x |= (x >> 1);
   x |= (x >> 2);
   x |= (x >> 4);
   x |= (x >> 8);
   x |= (x >> 16);

   return ++x;
}

template<typename T>
void bitonic_sort(T *base, size_t num,
                  int (*cmp)(const void *, const void *)) {
    T *d_base;
    cudaDeviceProp prop;
    size_t boundary = clp2(num);
    std::cout << boundary << std::endl;
    T pad;

    cudaSafeCall( cudaMalloc(&d_base, boundary * sizeof(T)) );
    cudaSafeCall (
        cudaMemcpy(d_base, base, num * sizeof(T), cudaMemcpyHostToDevice) );

    if ( boundary != num ) {
        pad = maximum<T>(base, num);

        std::cout << "Padding array with " << boundary - num << " more " 
                  << pad << " after first " << num << " elements" << std::endl;
        // Pad our array to the next highest power of two (elements)
        cudaSafeCall( cudaMemset(d_base + num, pad, boundary - num) );
    }

    cudaSafeCall( cudaGetDeviceProperties(&prop, 0) );
    dim3 threads(prop.maxThreadsPerBlock);
    dim3 blocks(max((int) boundary / prop.maxThreadsPerBlock, 1));

    for ( int k = 2; k <= boundary; k <<= 1 ) {
        std::cout << "k: " << k << std::endl;
        for ( int j = k >> 1; j > 0; j >>= 1 ) {
            std::cout << "j: " << j << std::endl;
            bitonic_kernel<<<blocks, threads>>>(d_base, boundary, j, k, cmp);
            cudaCheckError();
        }
    }

    cudaSafeCall( 
        cudaMemcpy(base, d_base, num * sizeof(T), cudaMemcpyDeviceToHost) );
    cudaSafeCall( cudaFree(d_base) );
}

template void bitonic_sort(unsigned *, size_t, 
                           int (*cmp)(const void *, const void *));


#endif // __BITONIC_KERNEL_CU__