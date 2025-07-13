#pragma once

#include "stddef.h"
#include "stdlib.h"

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