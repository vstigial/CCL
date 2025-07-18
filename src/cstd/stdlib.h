#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#define malloc(bytes)       HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, (bytes))
#define realloc(ptr, size)  HeapReAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, (ptr), (size))
#define free(ptr)           HeapFree(GetProcessHeap(), 0, (ptr))
#define exit(code)          ExitProcess((code));