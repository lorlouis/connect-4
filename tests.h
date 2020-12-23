#ifndef TESTS_H
#define TESTS_H 1

#include <stdio.h>

#define testing(str, fn) {\
    puts(str);\
    fn();\
    puts("\033[32m[OK]\033[0m");\
}

#endif
