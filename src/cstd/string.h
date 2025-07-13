#pragma once

#include "stddef.h"
#include "stdlib.h"
#include "stdbool.h"

size_t strlen(const char* str) {
    size_t len = 0;
    while (*str != '\0') {
        len++;
        str++;
    }
    return len;
}

char *substr(const char *string, size_t start, size_t len) {
    size_t string_length = strlen(string);

    if (start > string_length) {
        char *empty = malloc(1);
        empty[0] = '\0';
        return empty;
    }

    if (start + len > string_length) {
        len = string_length - start;
    }

    char *output = malloc(len + 1);

    for (size_t i = 0; i < len; ++i)
        output[i] = string[start + i];

    output[len] = '\0';
    return output;
}

bool streq(const char *str1, const char *str2) {
    size_t length1 = strlen(str1);
    if (length1 != strlen(str2)) { // early exit if true
        return false;
    }

    for (size_t i = 0; i < length1; i++) {
        if (str1[i] != str2[i]) // lengths of the strings are already verified in the above if statement, so this behavior is safe & defined
            return false;
    }

    return true;
}