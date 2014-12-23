#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

bool unique (char *str) {
    int *char_count = (int *) calloc(128, sizeof(int));

    for (char *c = str; *c != '\0'; c++) {
        if (char_count[*c] > 0) {
            return false;
        }
        char_count[*c]++;
    }

    return true;
}

bool unique_no_array (char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        for (int j = 0; j < i; j++) {
            if (str[j] == str[i]) {
                return false;
            }
        }
    }
    
    return true;
}

int main () {
    char unique_str[] = "no duplicates\n";
    char dup_str[] = "not quite unique\n";
    
    if (unique(unique_str)) {
        printf("%s is unique\n", unique_str);
    } else {
        printf("%s is not unique\n", unique_str);
    }
    if (unique(dup_str)) {
        printf("%s is unique\n", dup_str);
    } else {
        printf("%s is not unique\n", dup_str);
    }
    if (unique_no_array(unique_str)) {
        printf("%s is unique\n", unique_str);
    } else {
        printf("%s is not unique\n", unique_str);
    }
    if (unique_no_array(dup_str)) {
        printf("%s is unique\n", dup_str);
    } else {
        printf("%s is not unique\n", dup_str);
    }
}
