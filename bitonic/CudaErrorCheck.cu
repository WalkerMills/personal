/* Based on:
 *     https://gist.github.com/ashwin/2652488#file-cudaerrorcheck-cu
 */

#ifndef __CUDA_ERROR_CHECK_CU__
#define __CUDA_ERROR_CHECK_CU__

#include <iostream>
#include <cstdlib>

#include <cublas.h>
#include <cuda_runtime.h>
#include <curand.h>


// Define this to turn on error checking
#define CUDA_ERROR_CHECK
// Define this to turn on synchronous error checking
// #define CHECK_SYNC

// Wrapper macros
#define cudaSafeCall( err ) __cudaSafeCall( err, __FILE__, __LINE__ )
#define curandSafeCall( err ) __cuSafeCall( err, CURAND_STATUS_SUCCESS, __FILE__, __LINE__ )
#define cublasSafeCall( err ) __cuSafeCall( err, CUBLAS_STATUS_SUCCESS, __FILE__, __LINE__ )
#define cudaCheckError()    __cudaCheckError( __FILE__, __LINE__ )

// Safely make a CUDA API call
inline void __cudaSafeCall(cudaError err, const char *file, const int line,
                           bool abort_=true) {
#ifdef CUDA_ERROR_CHECK
    if ( err != cudaSuccess ) {
        std::cerr << "cudaSafeCall() failed at " << file << ":" << line
                  << " : " << cudaGetErrorString(err) << std::endl;

        if ( abort_ ) exit(EXIT_FAILURE);
    }
#endif // CUDA_ERROR_CHECK

    return;
}

inline void __cuSafeCall(int err, int success, char *file, int line,
                         bool abort_=true) {
#ifdef CUDA_ERROR_CHECK
    if ( err != success ) {
        std::cerr << "cuSafeCall failed at " << file << ":" << line
                  << " with code " << err << std::endl;
                  
        if ( abort_ ) exit(EXIT_FAILURE);
    }
#endif
}

// Check if the last kernel call failed
inline void __cudaCheckError(const char *file, const int line,
                             bool abort_=true) {
#ifdef CUDA_ERROR_CHECK
    cudaError err = cudaGetLastError();
    if ( err != cudaSuccess ) {
        std::cerr << "cudaCheckError() failed at " << file << ":" << line
                  << " : " << cudaGetErrorString(err) << std::endl;

        if ( abort_ ) exit(EXIT_FAILURE);
    }

#ifdef CHECK_SYNC
    // More careful checking. However, this will affect performance.
    err = cudaDeviceSynchronize();
    if ( err != cudaSuccess ) {
        std::cerr << "cudaCheckError() with sync failed at " << file << ":" 
                  << line << " : " << cudaGetErrorString(err) << std::endl;

        if ( abort_ ) exit(EXIT_FAILURE);
    }
#endif // CHECK_SYNC
    
#endif // CUDA_ERROR_CHECK

    return;
}

#endif // __CUDA_ERROR_CHECK_CU__