#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

char toupper(char in) {
    return in & 0xDF;
}

char tolower(char in) {
    return in | 0x20;
}

int CharToInt(char *str) {
    int res = 0;

    // Extract the low order nibbles from the string into decimal places
    while ( *str != '\0' ) {
        res *= 10;
        res += *str++ & 0x0F;
    }

    return res;
}

uint16_t ToDate(unsigned char month, unsigned char day, uint16_t year) {
    uint16_t date = 0;

    // Extract the last two decimal digits from year
    year %= 100;

    // Mask out uneeded bits
    month &= 0x0F;
    day &= 0x1F;

    // Pack date fields
    date ^= month;
    date <<= 5;
    date ^= day;
    date <<= 7;
    date ^= year;

    return date;
}

unsigned char ExtractMonth(uint16_t date) {
    // Extract bits [15,12]
    return date >> 12;
}

unsigned char ExtractDay(uint16_t date) {
    // Extract bits [11,7]
    return (date >> 7) & 0x1F;
}

unsigned char ExtractYear(uint16_t date) {
    // Extract bits [6,0]
    return date & 0x7F;
}

int CntBits(uint16_t x) {
    int nbits = 0;

    while (x != 0) {
        // Increment counter by the low order bit of x
        nbits += x & 1;

        // Right shift next bit into low order position
        x >>= 1;
    }

    return nbits;
}

bool TestBit(uint16_t word, uint16_t n) {
    if ( n > 15 ) return false;

    return (word >> n) & 1;
}

int16_t ROL(int16_t n) {
    int16_t mask = TestBit(n, 0);

    return (n << 1) | mask;
}

int16_t ROR(int16_t n) {
    int16_t mask = TestBit(n, 15);

    return (n >> 1) | mask;
}

int main() {
    char lower = 'e';
    char upper = 'E';

    printf("toupper(%c) = %c\n", lower, toupper(lower));
    printf("tolower(%c) = %c\n\n", upper, tolower(upper));

    char *number = "123";

    printf("CharToInt(\"%s\") = %d\n\n", number, CharToInt(number));

    unsigned char month = 7;
    unsigned char day = 9;
    uint16_t year = 2014;
    uint16_t date = ToDate(month, day, year);

    printf("Date in: %d/%d/%d\n", month, day, year);
    printf("Packed value: 0x%X\n", date);
    printf("Date out: %d/%d/%d\n\n", ExtractMonth(date), ExtractDay(date), 
           ExtractYear(date));

    int nbits = CntBits(date);

    printf("Bits set in 0x%X: %d\n\n", date, nbits);

    int bit = 4;
    int n = 2;

    printf("TestBit(0x%X, %d) = %s\n\n", bit, n, 
           TestBit(bit, n) ? "true" : "false");

    int16_t rotate = -3;

    printf("ROR(0x%X) = %d\n", rotate, ROR(rotate));
    printf("ROL(0x%X) = %d\n\n", rotate, ROL(rotate));

    return 0;
}
