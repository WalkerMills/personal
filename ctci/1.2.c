#include <stdlib.h>
#include <stdio.h>

char *reverse (char *str) {
    int i = 0;
    for ( ; str[i] != '\0'; i++) {
        continue;
    }
    char *rev = (char *) calloc (i, sizeof(char));
    int length = --i;

    for ( ; i >= 0; i--) {
        rev[length - i] = str[i];
    }

    return rev;
}

int main() {
    char str[] = "reverse this";
    char *reversed = reverse(str);
    printf("%s\n%s\n", str, reversed);
}
