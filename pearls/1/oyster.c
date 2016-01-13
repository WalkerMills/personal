#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


void gen_input(const char *file, int max, int ndata) {
    FILE *output = fopen(file, "w");
    int *numbers = (int *) calloc(max, sizeof(int));
    int i, random, tmp;

    srand(time(NULL));

    // Initialize the array of numbers
    for ( i = 0; i < max; ++i ) {
        numbers[i] = i;
    }

    for ( i = 0; i < ndata; ++i ) {
        // Choose a random index in the range [i, max)
        random = rand() % (max - i) + i;
        // Swap the random item into the ith position
        tmp = numbers[i];
        numbers[i] = numbers[random];
        numbers[random] = tmp;
        // Write the number to the output file
        fprintf(output, "%d\n", numbers[i]);
    }
    fclose(output);
    free(numbers);
}

#define BITS 16
#define WORD uint16_t

int *bitmap_sort(const char *infile, const char *outfile, int max) {
    int datum, i, j, map_size = ceil((double) max / (double) BITS);
    WORD *bitmap = (WORD *) calloc(map_size, sizeof(WORD));
    FILE *input, *output;

    input = fopen(infile, "r");
    while ( fscanf(input, "%d", &datum) != EOF ) {
        bitmap[datum / BITS] ^= 1 << (datum % BITS);
        // Check for duplicate input
        // if ( bitmap[datum / BITS] & (1 << (datum % BITS)) == 0 ) exit(1);
    }
    fclose(input);

    output = fopen(outfile, "w");
    // For each word
    for ( i = 0; i < map_size; ++i ) {
        // For each bit in the word
        for ( j = 0; j < BITS; ++j ) {
            // If it is set
            if ( bitmap[i] & (1 << j) ) {
                // Write the corresponding number to the output file
                fprintf(output, "%d\n", i * BITS + j);
            }
        }
    }
    fclose(output);
    free(bitmap);
}

int main() {
    int max = 10000000;
    int n = 1000000;
    const char *infile = "input.txt";
    const char *outfile = "output.txt";

    gen_input(infile, max, n);
    bitmap_sort(infile, outfile, max);

    return 0;
}