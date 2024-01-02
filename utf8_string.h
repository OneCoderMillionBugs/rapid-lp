#ifndef UTF8_STRING
#define UTF8_STRING

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "error.h"

struct utf8_ch
{
    uint32_t code : 21; // unicode code point
    uint32_t size : 3;  // size of a char
    uint32_t dummy : 8; // reserved for future use
}
typedef utf8_char;

struct utf8_str
{
    utf8_char *chars;   // code representations
    size_t size;        // size in bytes
    size_t length;      // length of string
}
typedef utf8_string;

/**
 * Read raw utf-8 string, check each byte 
 * for validity, and store them in fixed size slots
 */
utf8_string *utf_read(uint8_t *bytes, size_t size);

/**
 * Free all memory occupied by the object
 * Highly recommended to set pointer on which this function was called to NULL!
 */
void utf_destruct(utf8_string *string);

#endif