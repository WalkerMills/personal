#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool isSubstring (char *s1, char *s2) {
    if (strcmp(s1, s2) == 0) {
        return true;
    } else {
        return false;
    }
}

bool is_rotation (char *s1, char *s2) {
    char *buf;

    if (strlen(s1) != strlen(s2)) {
        return false;
    } else {
        buf = (char *) calloc(strlen(s1) + 1, sizeof(char));
    }

    for (int i = 0; i < strlen(s1); i++) {
        strcpy(buf, s2 + i);
        strncpy(buf + strlen(s2 + i), s2, i);

        if (isSubstring(buf, s1)) {
            return true;
        }
    }

    return false;
}

int main () {
    char *s1 = "waterbottle";
    char *s2 = "erbottlewat";

    if (is_rotation(s1, s2)) {
        printf("%s is a rotation of %s\n", s2, s1);
    }

    return 0;
}
