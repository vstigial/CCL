#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#define malloc(bytes)       HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, (bytes))
#define realloc(ptr, size)  HeapReAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, (ptr), (size))
#define free(ptr)           HeapFree(GetProcessHeap(), 0, (ptr))
#define exit(code)          ExitProcess((code))

static long parse_decimal(const char *s) {
    if (!s || *s == '\0') return -1;

    long result = 0;
    while (*s) {
        char c = *s++;
        if (c < '0' || c > '9') {
            return -1;
        }
        result = result * 10 + (c - '0');
    }
    return result;
}

// supports leading '-'
static long parse_int_literal(const char *s) {
    if (!s || *s == '\0') return -1;

    char negative = 0;
    if (*s == '-') {
        negative = 1;
        s++;
    }
    long v = parse_decimal(s);
    return (v < 0) ? -1 : (negative ? -v : v);
}