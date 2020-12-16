#include <stdio.h>
#include <string.h>

#include "util.h"

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

void get_string_after(char *original, char delimiter, char *result) {
  unsigned length = strlen(original);

  unsigned resByte = 0;
  for (unsigned i = 0; i < length; ++i) {
    char c = original[i];

    if (c == delimiter)
      resByte = 0;
    else
      result[resByte++] = c;
  }
}