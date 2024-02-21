#include "utf8_string.h"

// Get the size of a character
static uint8_t utf_charsz(uint8_t byte)
{
    // Mappings of all possible 5 bit prefixes to character sizes
    static const uint8_t sizes[32] = {
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
        0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 3, 3, 4, 0
    };

    if (sizes[byte >> 3] == 0)
    {
        print_error("Invalid utf-8 character");
    }

    return sizes[byte >> 3];
}

/**
 * Calculates codepoint for encoded character
 */
static uint32_t utf_decode(uint32_t encoded, uint8_t length)
{
    // Masks
    static const uint32_t byteone = 0x7F;
    static const uint32_t bytetwo[5] = {0x0, 0x0, 0x1F00, 0x3F00, 0x3F00};
    static const uint32_t bytethree[5] = {0x0, 0x0, 0x0, 0xF0000, 0x3F0000};
    static const uint32_t bytefour[5] = {0x0, 0x0, 0x0, 0x0, 0x7000000};
    static const uint8_t shifts[5] = {0, 0, 2, 2, 2};

    uint32_t code = encoded & byteone;
    code |= (encoded & bytetwo[length]) >> shifts[length];
    code |= (encoded & bytethree[length]) >> 2 * shifts[length];
    code |= (encoded & bytefour[length]) >> 3 * shifts[length];

    return code;
}

/**
 * Returns encoded character representation from a given codepoint
 * TODO:
 */
static uint32_t utf_encode(uint32_t codepoint)
{
    return 0;
}

/**
 * Read the string in 2 passes (first pass for validation and length 
 * determination, second for reading and storing the data itself) and 
 * store it in code-points form
 */
utf8_string *utf_read(uint8_t *bytes, size_t size)
{
    utf8_string *string = (utf8_string *)malloc(sizeof(utf8_string));
    if (string == NULL)
    {
        print_error("'utf_read' can't allocate memory");
        return NULL;
    }
    size_t length = 0;

    // First pass: Count characters and validate them
    for (size_t i = 0; i < size; i++)
    {
        uint8_t charsz = utf_charsz(bytes[i]);

        while (charsz-- > 1)
        {
            if ((++i >= size) || ((bytes[i] & 0xC0) != 0x80))
            {
                print_error("Invalid utf-8 character");
                return NULL;
            }
        }
        length++;
    }

    // Second pass: Decode bytes to unicode code points
    utf8_char *chars = (utf8_char *)calloc(length, sizeof(utf8_char));
    if (chars == NULL)
    {
        print_error("'utf_read' can't allocate memory");
        return NULL;
    }
    size_t index = 0;

    for (size_t i = 0; i < size; i++)
    {
        uint8_t charsz = utf_charsz(bytes[i]);
        uint32_t encoded = bytes[i] << 24;

        for (size_t j = 2; j <= charsz; j++)
        {
            encoded |= bytes[++i] << (32 - j * 8);
        }
        encoded >>= 32 - charsz * 8;

        utf8_char ch = {
            .code = utf_decode(encoded, charsz), 
            .size = charsz, 
            .dummy = 0
        };

        chars[index++] = ch;
    }

    string->chars = chars;
    string->size = size;
    string->length = length;

    return string;
}

void utf_destruct(utf8_string *string)
{
    if (string == NULL)
    {
        print_error("'utf_destruct' got null ptr");
        return;
    }

    free(string->chars);
    free(string);
}