#include "rapidlp.h"

# define MAX_HASH 384

/**
 * Very specific hash function which doesn't produce any collisions 
 * for unicode characters in ranges [U+0000, U+007F] (ascii symbols) and 
 * [U+0400, U+04FF] (cyrillic symbols)
 * Those ranges will now be called "valid ranges"
 * 
 * So this function is possible to map 128 + 256 = 384 keys, 
 * hence the max valid index is 383
 * 
 * For characters outside of valid ranges it just returns 384, which is
 * equal to max valid index 383 + 1
 */
static uint32_t hash(uint32_t key)
{
    if (0 <= key && key < 128)
    {
        return key;
    }
    else if (1024 <= key && key < 1280)
    {
        return 128 + key - 1024;
    }
    return MAX_HASH;
}

static PyObject *shannon_ch(PyObject *self, PyObject *args)
{
    PyObject *string;

    if (!PyArg_ParseTuple(args, "U", &string))
    {
        return NULL;
    }

    size_t length = PyUnicode_GetLength(string);

    /**
     * Table size should be set to 385 since there 
     * are 384 different values which hash function 
     * could return, the last index is ignored in line 60, 
     * so the characters outside of specified range won't affect 
     * the final result
     */
    uint32_t table[MAX_HASH + 1] = {0};
    UINTList *keys = uintlist_create();

    for (size_t i = 0; i < length; i++)
    {
        uint32_t index = hash(PyUnicode_ReadChar(string, i));
        if (table[index] == 0 && index != MAX_HASH)
        {
            uintlist_add(keys, index);
        }
        table[index]++;
    }

    double entropy = 0;

    for (size_t i = 0; i < keys->size; i++)
    {
        uint32_t index = uintlist_get(keys, i);
        double freq = 1.0 * table[index] / length;
        if (freq > 0)
        {
            entropy += freq * log2(freq);
        }
    }

    uintlist_destruct(keys);
    return PyFloat_FromDouble(-1 * entropy);
}


#define MAX_SIZE 1000

static PyObject *jaro_similarity(PyObject *self, PyObject *args)
{
    PyObject *str_1;
    PyObject *str_2;

    if (!PyArg_ParseTuple(args, "UU", &str_1, &str_2))
    {
        return NULL;
    }

    int32_t len1 = PyUnicode_GetLength(str_1);
    int32_t len2 = PyUnicode_GetLength(str_2);

    int32_t d = fmax(len1, len2) / 2 - 1;
    int32_t n_matches = 0;

    uint8_t matches_1[MAX_SIZE] = {0};
    uint8_t matches_2[MAX_SIZE] = {0};

    for (size_t i = 0; i < len1; i++)
    {
        bool ismatch = false;

        for (size_t j = fmax(i - d, 0); j < fmin(len2, i); j++)
        {
            Py_UCS4 c1 = PyUnicode_ReadChar(str_1, i);
            Py_UCS4 c2 = PyUnicode_ReadChar(str_2, j);

            if (c1 == c2)
            {
                matches_1[i] = 1;
                matches_2[j] = 1;
                n_matches++;
                ismatch = true;
                break;
            }
        }

        if (ismatch) continue;

        for (size_t j = i; j < fmin(len2, i + d + 1); j++)
        {
            Py_UCS4 c1 = PyUnicode_ReadChar(str_1, i);
            Py_UCS4 c2 = PyUnicode_ReadChar(str_2, j);

            if (c1 == c2)
            {
                matches_1[i] = 1;
                matches_2[j] = 1;
                n_matches++;
                break;
            }
        }
    }

    uint32_t trans = 0;
    size_t j = 0;

    for (size_t i = 0; i < len1; i++)
    {
        if (!matches_1[i])
        {
            continue;
        }

        while (!matches_2[j] && j < len2)
        {
            j++;
        }

        if (j < len2 && matches_2[j] && 
            PyUnicode_ReadChar(str_1, i) != PyUnicode_ReadChar(str_2, j))
        {
            trans++;
        }
        j++;
    }

    trans /= 2;
    double jaro = (1.0 * n_matches / len1 + 
                   1.0 * n_matches / len2 + 
                   1.0 * (n_matches - trans) / n_matches) / 3;

    return PyFloat_FromDouble(jaro);
}


static PyMethodDef ModuleMethods[] = {
    {"shannon_ch", shannon_ch, METH_VARARGS,
     "Calculate Shannon entropy for word."},
    {"jaro_similarity", jaro_similarity, METH_VARARGS,
     "Calculate Jaro similarity between two words."},
    {NULL, NULL, 0, NULL}        /* Sentinel */
};

static struct PyModuleDef rapidlp = {
    PyModuleDef_HEAD_INIT,
    "rapidlp", /* name of module */
    NULL,       /* module documentation, may be NULL */
    -1,         /* size of per-interpreter state of the module,
                 or -1 if the module keeps state in global variables. */
    ModuleMethods
};

PyMODINIT_FUNC PyInit_rapidlp(void)
{
    return PyModule_Create(&rapidlp);
}