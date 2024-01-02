#ifndef ERROR_H
#define ERROR_H

#define RED_COLOR "\033[31m"
#define RESET_COLOR "\033[0m"

#include <stdio.h>
#include <stdlib.h>

static inline void write_errcode(unsigned char *ptr, unsigned char code)
{
    if (ptr != NULL)
    {
        *ptr = code;
    }
}

static inline void print_error(char *message)
{
    fprintf(stderr, "%sERROR: %s\n%s", RED_COLOR, message, RESET_COLOR);
}

#endif