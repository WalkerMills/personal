#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int compare (const void *a, const void *b) {
    char ia = *(char *)a;
    char ib = *(char *)b;
    return ia - ib;
}

void remove_dup_sort (char *str) {
    int length = strlen(str);

    if (length < 2) {
        return;
    } else {
        qsort(str, length, sizeof(char), compare);
    }

    for (int i = length - 1; i > 0; i--) {
        if (str[i] == str[i - 1]) {
            memmove(str + i - 1, str + i, strlen(str + i) + 1);
        }
    }
}

void remove_dup (char *str) {
    int length = strlen(str);

    if (length < 2) {
        return;
    }

    for (int i = length - 1; i > 0; i--) {
        if (strchr(str + i, str[i - 1]) != NULL) {
            memmove(str + i - 1, str + i, strlen(str + 1) + 1);
        }
    }
}

int main() {
    char *input = (char *) calloc(100, sizeof(char));
    char *copy;

    fgets(input, 100, stdin);
    input[strlen(input) - 1] = '\0';
    copy = (char *) calloc(strlen(input) + 1, sizeof(char));
    strcpy(copy, input);

    printf("%s\n", input);
    remove_dup_sort(input);
    printf("%s\n\n", input);

    printf("%s\n", copy);
    remove_dup(copy);
    printf("%s\n", copy);

    return 0;
}
