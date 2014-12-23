#ifndef __BITONIC_KERNEL_CUH__
#define __BITONIC_KERNEL_CUH__

#include <cuda_runtime.h>


template<typename T>
__global__ void bitonic_kernel(T *base, size_t num, int j, int k,
                               int (*cmp)(const void *, const void *));
template<typename T>
void bitonic_sort(T *base, size_t num, int (*cmp)(const void *, const void *));

#endif // __BITONIC_KERNEL_CUH__