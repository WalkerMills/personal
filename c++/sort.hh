#ifndef __SORT_HH__
#define __SORT_HH__

#include <algorithm>
#include <iterator>
#include <iostream>
#include <sstream>
#include <stack>
#include <utility>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <ctime>

#include <omp.h>

#include "timsort.hpp"

// The maximum size of an array which will choose the median as the pivot
// for partitioning
#define SMALL 120

// Number of elements that trigger insertion sort from quicksort by default
#define CHUNK 100

// If there are more than SMOOTH elements when introsort reaches maximum
// recursion depth, use smoothsort; otherwise, use shellsort
#define SMOOTH 10000

// Number of threads to use for multithreading quicksort
#define THREADS 3

// Set a default maximum recursion depth (for introsort) of 2 * log(n)
#define DEPTH(n) 2 * log2(n) + 1

// Type large enough to contain any Leonardo number
typedef uint32_t leonardo_t;

// Type large enough to contain any number in Ciura's shell sort gap sequence
typedef uint8_t gap_t;

// Struct for holding the sizes of existing heaps
typedef struct {
    // Sizes of existing heaps (right shifted so that the last bit is a 1)
    uint64_t mask;
    // Size of the smallest heap (mask << offset == unshifted mask)
    uint32_t offset;
} heapsize_t;


namespace sort {
    // Number of Leonardo numbers smaller than 2^32
    const uint8_t num_leonardo = 46;

    // Precomputed array of Leonardo numbers smaller than 2^32
    const leonardo_t leonardo_numbers[num_leonardo] = {
        1UL, 1UL, 3UL, 5UL, 9UL, 15UL, 25UL, 41UL, 67UL, 109UL, 177UL, 287UL, 
        465UL, 753UL, 1219UL, 1973UL, 3193UL, 5167UL, 8361UL, 13529UL, 
        21891UL, 35421UL, 57313UL, 92735UL, 150049UL, 242785UL, 392835UL, 
        635621UL, 1028457UL, 1664079UL, 2692537UL, 4356617UL, 7049155UL, 
        11405773UL, 18454929UL, 29860703UL, 48315633UL, 78176337UL, 
        126491971UL, 204668309UL, 331160281UL, 535828591UL, 866988873UL, 
        1402817465UL, 2269806339UL, 3672623805UL
    };

    // Number of elements in the shell sort gap sequence (Ciura, 2001)
    const uint8_t num_gaps = 6;

    // Precomputed gap sequence array
    const gap_t gaps[num_gaps] = {
        132U, 57U, 23U, 10U, 4U, 1U
    }; 

    // Numeric type (with ++ and -- overloads) for keeping track of
    // Djikstra's b and c for smoothsort (produces precomputed values
    // above)
    class leonardo {
    private:
        // Current Leonardo number
        leonardo_t b;
        // Previous Leonardo number
        leonardo_t c;

    public:
        // Default constructor
        leonardo() throw() : b(1), c(1) { 
            return; 
        }
        // Copy constructor
        leonardo(const leonardo &l) throw() : b(l.b), c(l.c) { 
            return; 
        }

        // Calculate the gap between the current and previous Leonardo numbers
        unsigned gap() const throw() {
            return b - c;
        }

        // Increment to the next Leonardo number
        leonardo &operator++() throw() {
            std::swap(b, c);
            b += c + 1;
            
            return *this;
        }

        // Decrement to the previous Leonardo number
        leonardo &operator--() throw() {
            std::swap(b, c);
            c -= b + 1;

            return *this;
        }

        // Return current value
        operator leonardo_t() const throw() {
            return b;
        }

        // Return previous value
        leonardo_t operator~() const throw() {
            return c;
        }
    };


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
        sort::print(input.begin(), input.end());
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
        return sort::check(input.begin(), input.end());
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
        return sort::show(input.begin(), input.end());
    }


    // In-place insertion sort on the elements in the range [first, last)
    template<typename T>
    void insertion_sort(T start, T stop) {
        T left, right, center;
        unsigned step;

        for ( T i = start + 1; i < stop; ++i ) {
            auto value = *i;
            right = i;
            step = 1;

            // Assuming that the elements in the range [start, i) are sorted,
            // find the correct insertion point
            for ( left = start; left + step <= right; ) {
                // Start near i and take increasingly large steps
                center = right - step;
                step <<= 1;

                if ( value < *center ) {
                    right = center;
                } else {
                    left = center + 1;
                }
            }

            // Perform a binary search
            while ( left < right ) {
                center = left + (right - left) / 2;

                if ( value < *center ) {
                    right = center;
                } else {
                    left = center + 1;
                }
            }

            // Chain swap elements into place
            T j = i;
            for ( ; j > left; --j ) {
                *j = *(j - 1);
            }
            // Put the original value in the correct position
            *j = value;
        }
    }

    // In-place insertion sort on all the elements in the container
    template<typename T>
    void insertion_sort(T &input) {
        sort::insertion_sort(input.begin(), input.end());
    }

    // In-place insertion sort on the first n elements of the input array
    template<typename T>
    void insertion_sort(T *input, unsigned size) {
        sort::insertion_sort(input, input + size);
    }


    template<typename T>
    inline void phase(T start, T stop, gap_t gap) {
        for ( T i = start + gap; i < stop; ++i ) {
            auto value = *i;

            // Chain swap elements into place
            T j = i - gap;
            for ( ; j >= start && *j > value; j -= gap ) {
                *(j + gap) = *j;
            }
            // Put the original value in the correct position
            *(j + gap) = value;
        }
    }

    template<typename T>
    void shellsort(T start, T stop) {
        for ( uint8_t i = 0; i < num_gaps; ++i ) {
            sort::phase(start, stop, sort::gaps[i]);
        }
    }

    template<typename T>
    void shellsort(T &input) {
        sort::shellsort(input.begin(), input.end());
    }

    template<typename T>
    void shellsort(T *input, unsigned size) {
        sort::shellsort(input, input + size);
    }


    // Rebalance the tree at the given root
    template<typename T>
    inline void sift(T root, unsigned size) {
        T left, right, next;
        unsigned child_size;

        // Store original value
        auto value = *root;

        // We are in a leaf of the tree, so return
        if ( size < 2 ) return;

        do {
            // Find right child
            right = root - 1;
            // Find left child
            left = right - sort::leonardo_numbers[size - 2];

            // Compare the children's roots
            if ( *left < *right ) {
                // Choose right child
                next = right;
                child_size = size - 2; 
            } else {
                // Choose left child
                next = left;
                child_size = size - 1;
            }

            // If both of the children's roots are less than the original
            // value, then root is at the correct position
            if ( *next <= value ) break;

            // Push the larger root up the tree by chain swapping
            *root = *next;

            // Move down the tree
            root = next;
            size = child_size;
        } while ( size > 1 );

        // Put the original value in the correct position
        *root = value;
    }

    // Rebalance and sort the roots of the heaps in the tree at the given root 
    template<typename T>
    inline void trinkle(T root, heapsize_t size) {
        T left, right, next;

        // Store original value
        auto value = *root;

        // Traverse the implicit list of heaps from right to left
        while ( size.mask != 1 ) {
            auto max = value;

            // If this heap contains any subheaps
            if ( size.offset > 1 ) {
                // Find right child
                right = root - 1;
                // Find left child
                left = right - sort::leonardo_numbers[size.offset - 2];

                // Use the maximum of *left, *right, and value for any
                // comparisons
                if ( max < *left ) max = *left;
                if ( max < *right ) max = *right;
            }

            // Find the next heap
            next = root - leonardo_numbers[size.offset];

            // If it's already in order, stop
            if ( *next <= max ) break;

            // Push the larger root up the tree by chain swapping
            *root = *next;

            // Move down the tree
            root = next;

            // Get the previous heap from the implicit list
            do {
                size.mask >>= 1;
                ++size.offset;
            } while ( ! (size.mask & 1) );
        }

        // Put the original value in the correct position
        *root = value;

        // Make sure that the heap we stopped at is ordered properly
        sort::sift(root, size.offset);
    }

    // Heapify the elements in the range [start, stop)
    template<typename T>
    inline heapsize_t heapify(T start, T stop) {
        heapsize_t size;

        // Indicates whether a heap will need to be fused later
        bool fuse;

        size.mask = 1;
        size.offset = 1;

        for ( T i = start; i < stop; ++i ) {
            // If the sizes of the last two heaps are contiguous Leonardo
            // numbers
            if ( size.mask & 2 ) {
                // Fuse the last two heaps
                size.mask = (size.mask >> 2) | 1;
                size.offset += 2;
            // If the last heap is of size leonardo_numbers[1]
            } else if ( size.offset == 1 ) {
                // Make a new heap of size leonardo_numbers[0]
                size.mask = (size.mask << 1) | 1;
                size.offset = 0;
            } else {
                // Otherwise, make a new heap of size leonardo_numbers[1]
                size.mask = (size.mask << (size.offset - 1)) | 1;
                size.offset = 1;
            }

            // If the sizes of the last two heaps are contiguous Leonardo
            // numbers and there's at least one more element in the range,
            // or the size of this heap is a Leonardo number and there are
            // at least as many elements remaining in the range as the
            // previous Leonardo number, then this heap will need to be fused
            fuse = ((size.mask & 2) && (i + 1 < stop)) || 
                   ((size.offset > 0) && 
                    (i + leonardo_numbers[size.offset - 1] + 1 < stop));

            if ( fuse ) {
                // If it needs to be fused eventually, we can just do a sift
                sort::sift(i, size.offset);
            } else {
                // Otherwise, it is a final heap, so we need to do a trinkle
                sort::trinkle(i, size);
            }
        }

        return size;
    }

    // Extract all elements from the (heapified) range [start, stop)
    template<typename T>
    inline void extract(T start, T stop, heapsize_t size) {
        // Extract all elements except for the last two
        for ( T i = stop - 1; i > start + 1; --i ) {
            // If the last heap only has one element, ignore it
            if ( size.offset < 2 ) {
                do {
                    size.mask >>= 1;
                    ++size.offset;
                } while ( ! (size.mask & 1) );
            } else {
                // Remove this heap
                size.mask &= ~1ULL;

                // Add the left child and make sure its roots are sorted
                size.mask = (size.mask << 1) | 1;
                sort::trinkle(i - leonardo_numbers[--size.offset - 1] - 1,
                              size);

                // Add the right child and make sure its roots are sorted
                size.mask = (size.mask << 1) | 1;
                --size.offset;
                sort::trinkle(i - 1, size);
            }
        }
    }


    // In-place smoothsort on the elements in the range [start, stop)
    template<typename T>
    void smoothsort(T start, T stop) {
        if ( stop - start < 2 ) return;

        heapsize_t size = sort::heapify(start, stop);
        sort::extract(start, stop, size);
    }

    // In-place smoothsort on all the elements in the container
    template<typename T>
    void smoothsort(T &input) {
        if ( input.size() < 2 ) return;

        heapsize_t size = sort::heapify(input.begin(), input.end());
        sort::extract(input.begin(), input.end(), size);
    }

    // In-place smoothsort sort on all the elements in an array
    template<typename T>
    void smoothsort(T *input, unsigned n) {
        if ( n < 2 ) return;

        heapsize_t size = sort::heapify(input, input + n);
        sort::extract(input, input + n, size);
    }


    // Calculate the median of three values
    template<typename T>
    inline T medianOf3(T a, T b, T c) {
        // Find the maximum of all three elements
        T maximum = std::max(std::max(a, b), c);

        // Find the minimum of all three elements
        T minimum = std::min(std::min(a, b), c);

        // xor'ing all these values together will zero all but the median
        return a ^ b ^ c ^ minimum ^ maximum;
    }

    // Calculate Tukey's ninther of nine values
    template<typename T>
    inline T ninther(T a, T b, T c, T d, T e, T f, T g, T h, T i) {
        T med0 = sort::medianOf3(a, b, c);
        T med1 = sort::medianOf3(d, e, f);
        T med2 = sort::medianOf3(g, h, i);

        return sort::medianOf3(med0, med1, med2);
    }

    // Places pivot at the beginning of the range [start, stop)
    template<typename T>
    inline void place_pivot(T start, T stop) {
        unsigned n = stop - start;
        auto pivot = *start;

        if ( n <= SMALL ) {
            // Find the median of the first, middle, and last elements of
            // small ranges
            pivot = sort::medianOf3(*start, *(start + n / 2), *(stop - 1));
        } else {
            int part = n / 8;
            T mid = start + n / 2;

            // Compute Tukey's ninther
            pivot = sort::ninther(
                *start, *(start + part), *(start + part + part), 
                *(mid - part), *mid, *(mid + part), 
                *(stop - 1 - part - part), *(stop - 1 - part), *(stop - 1));
        }

        // Swap it to the beginning of the range
        std::swap(*start, pivot);

    }

    template<typename T>
    inline std::pair<T, T> ungarded_partition(T start, T stop){
        sort::place_pivot(start, stop);

        // Bentley-McIlroy three-way partitioning
        T i = start, p = start;
        T j = stop, q = stop;

        while ( true ) {
            // Increment i while its value is less than the pivot
            while ( *++i < *start ) if ( i == stop - 1 ) break;

            // Decrement j while its value is greater than the pivot
            while ( *start < *--j ) if ( j == start ) break;

            // i and j have crossed
            if ( i == j && *i == *start ) std::iter_swap(i, ++p);

            // i has passed j
            if ( i >= j ) break;

            // Swap i and j
            std::iter_swap(i, j);

            // Move pivot values to the ends of the range
            if ( *i == *start ) std::iter_swap(i, ++p);
            if ( *j == *start ) std::iter_swap(j, --q);
        }

        // Swap pivot values into place in the array such that all preceding
        // elements are smaller, and all following elements are larger
        i = j + 1;
        for ( T k = start; k <= p; ++k ) std::iter_swap(k, j--);
        for ( T k = stop - 1; k >= q; --k ) std::iter_swap(k, i++);

        // Return the (inclusive) indices of the range that has been sorted
        return std::make_pair(j + 1, i);
    }

    // Quicksort partitioning function
    template<typename T>
    inline std::pair<T, T> partition(T start, T stop, unsigned chunk) {
        int n = stop - start;

        if ( n <= 1 ) {
            return std::make_pair(start, stop);
        } else if ( n == 2 ) {
            if ( *start > *(stop - 1) ) std::iter_swap(start, stop - 1);
            return std::make_pair(start, stop);
        } else if ( n <= chunk ) {
            // Perform insertion sort on any chunk smaller than the given
            // cutoff
            sort::shellsort(start, stop);
            return std::make_pair(start, stop);
        }
        return ungarded_partition(start, stop);
    }

    // In-place quicksort on the elements in the range [start, stop), with the
    // given chunk size
    template<typename T>
    void quicksort(T start, T stop, unsigned chunk) {
        // Make a stack for holding pairs of iterators representing the
        // non-inclusive range [first, second)
        std::stack<std::pair<T, T>> st;

        // Push a terminal element onto the stack
        st.push(std::make_pair(start, start));

        while ( ! st.empty() ) {
            while ( start < stop ) {
                // Partition the given range using the default chunk size
                std::pair<T, T> indices = sort::partition(start, stop, CHUNK);

                // Push the range which includes elements larger than the
                // pivot onto the stack
                st.push(std::make_pair(indices.second, stop));

                // Set the end of the range to be the end of the range
                // which includes elements smaller than the pivot
                stop = indices.first;
            }

            // Retrieve the next range from the stack
            std::pair<T, T> indices = st.top();
            st.pop();

            // Set the next range
            start = indices.first;
            stop = indices.second;
        }
    }

    // In-place quicksort on the elements in the range [start, stop)
    template<typename T>
    void quicksort(T start, T stop) {
        sort::quicksort(start, stop, CHUNK);
    }

    // In-place quicksort on all the elements in the container
    template<typename T>
    void quicksort(T &input) {
        sort::quicksort(input.begin(), input.end(), CHUNK);
    }

    // In-place quicksort on the first n elements of the input array
    template<typename T>
    void quicksort(T *input, unsigned n) {
        sort::quicksort(input, input + n, CHUNK);
    }

    template<typename T>
    inline void parallel_quicksort(T start, T stop, unsigned const nthread) {
        if ( stop <= start ) {
            return;
        }
        unsigned part = (stop - start) / nthread;

        #pragma omp parallel for default(shared)
        for ( unsigned i = 0; i < nthread - 1; ++i ) {
            sort::quicksort(start + i * part, start + (i + 1) * part);
        }

        sort::quicksort(start + (nthread - 1) * part, stop); 
        gfx::timsort(start, stop);
    }

    // In-place (parallel) quicksort on the elements in the range [start, stop)
    template<typename T>
    void parallel_quicksort(T start, T stop) {
        sort::parallel_quicksort(start, stop, THREADS);
    }


    // In-place introsort on the elements in the range [start, stop) with
    // parametrized depth 
    template<typename T>
    void introsort(T start, T stop, unsigned depth) {
        // Make a stack for holding pairs of iterators representing the
        // non-inclusive range [first, second)
        std::stack<std::pair<T, T>> *st = new std::stack<std::pair<T, T>>();

        // Push a terminal element onto the stack
        st->push(std::make_pair(start, start));

        while ( ! st->empty() ) {
            while ( start < stop ) {
                if ( --depth < 1 ) {
                    // If we've exceeded the maximum depth, smoothsort the
                    // remaining elements
                    if ( stop - start > SMOOTH ) {
                        sort::smoothsort(start, stop);
                    } else {
                        sort::shellsort(start, stop);
                    }
                    break;
                }

                // Partition the given range using the default chunk size
                std::pair<T, T> indices = sort::partition(start, stop, CHUNK);

                // Push the range which includes elements larger than the
                // pivot onto the stack
                st->push(std::make_pair(indices.second, stop));

                // Set the end of the range to be the end of the range
                // which includes elements smaller than the pivot
                stop = indices.first;
            }

            // Retrieve the next range from the stack
            std::pair<T, T> indices = st->top();
            st->pop();

            // Set the next range
            start = indices.first;
            stop = indices.second;
        }

        delete st;
    }

    // In-place introsort on the elements in the range [start, stop)
    template<typename T>
    void introsort(T start, T stop) {
        sort::introsort(start, stop, DEPTH(stop - start));
    }

    // In-place introsort on the input container
    template<typename T>
    void introsort(T &input) {
        sort::introsort(input.begin(), input.end(), DEPTH(input.size()));
    }

    // In-place introsort on the first n elements of the input array
    template<typename T>
    void introsort(T *input, unsigned n) {
        sort::introsort(input, input + n, DEPTH(n));
    }

    template<typename T>
    inline void parallel_introsort(T start, T stop, unsigned const nthread) {
        if ( stop <= start ) {
            return;
        }
        unsigned part = (stop - start) / nthread;
        unsigned depth = DEPTH(part);

        #pragma omp parallel for default(shared)
        for ( unsigned i = 0; i < nthread - 1; ++i ) {
            sort::introsort(start + i * part, start + (i + 1) * part, depth);
        }

        sort::introsort(start + (nthread - 1) * part, stop, depth); 
        gfx::timsort(start, stop);
    }

    // In-place (parallel) introsort on the elements in the range [start, stop)
    template<typename T>
    void parallel_introsort(T start, T stop) {
        sort::parallel_introsort(start, stop, THREADS);
    }
}

#endif // __SORT_HH__
