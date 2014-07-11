#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int compare (const void *a, const void *b) {
    char ia = *(char *)a;
    char ib = *(char *)b;
    return ia - ib;
}

bool anagrams (char *str1, char *str2) {
    qsort(str1, strlen(str1), sizeof(char), compare);
    qsort(str2, strlen(str2), sizeof(char), compare);

    for (int i = 0; str1[i] == '\0' && str2[i] == '\0'; i ++) {
        if (str1[i] != str2[i]) {
            return false;
        }
    }
    return true;
}

int main() {
    char *buf1 = (char *) calloc(96, sizeof(char));
    char *buf2 = (char *) calloc(96, sizeof(char));
    char *copy1 = (char *) calloc(96, sizeof(char));
    char *copy2 = (char *) calloc(96, sizeof(char));

    fgets(buf1, 95, stdin);
    fgets(buf2, 95, stdin);
    strcpy(copy1, buf1);
    strcpy(copy2, buf2);

    if (anagrams(buf1, buf2)) {
        printf("\n%sand\n%sare anagrams\n", copy1, copy2);
    }

    return 0;
}
