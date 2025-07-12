#pragma once

#define isspace(c) (((c) == ' ') || ((c) == '\t') || ((c) == '\n') || ((c) == '\v') || ((c) == '\f') || ((c) == '\r'))
#define isalpha(c) ((((c) >= 'A') && ((c) <= 'Z')) || (((c) >= 'a') && ((c) <= 'z')))
#define isdigit(c) (((c) >= '0') && ((c) <= '9'))