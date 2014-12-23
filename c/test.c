#include <stdio.h>

#define CONCAT(a, b) CONCAT_TWICE(a, b)
#define CONCAT_TWICE(a, b) a##b
#define printfl(format, ...) fprintf(stdout, CONCAT(format, " From line %d\n"), ##__VA_ARGS__, __LINE__)

int main() {
    const char *fmt = "Hello World! I have %d argument(s).";

    printfl(fmt, 1);
    return 0;
}
