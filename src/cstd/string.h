#pragma once

#include "stddef.h"

size_t strlen(const char* str) {
    size_t len = 0;
    while (*str != '\0') {
        len++;
        str++;
    }
    return len;
}

void substr(const char *string, char *output, size_t start, size_t len) {
    size_t string_length = strlen(string);

    if (start > string_length) {
        output[0] = '\0';
        return;
    }

    if (start + len > string_length) {
        len = string_length - start;
    }

    for (size_t i = 0; i < len; ++i)
        output[i] = string[start + i];

    output[len] = '\0';
}