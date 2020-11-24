#include "util.h"

#include <stdio.h>

bool substring(char* string, unsigned first, unsigned length, char* res) {
    int i = 0;

    while (i < length) {
        if (string[first + i] == '\0')
            return false;
        res[i] = string[first + i];
        i++;
    }

    res[i] = '\0';
    return true;
}

unsigned string_len(char* string) {
    unsigned i = 0;

    bool done = false;
    while (!done) {
        if (string[i] == '\0' || string[i] == '\r' || string[i] == '\n')
            done = true;
        i++;
    }

    return i - 1;
}