#include "nlptools.h"

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
    return 384;
}


static PyObject *shannon_ch(PyObject *self, PyObject *args)
{
    Py_buffer raw = {0};

    if (!PyArg_ParseTuple(args, "s*", &raw))
    {
        return NULL;
    }

    utf8_string *string = utf_read(raw.buf, raw.len);

    if (string->length == 0)
    {
        utf_destruct(string);
        PyBuffer_Release(&raw);
        return PyFloat_FromDouble(0);
    }

    /**
     * Table size should be set to 385 since there 
     * are 384 different values which hash function 
     * could return, the last index is ignored in line 60, 
     * so the characters outside of specified range won't affect 
     * the final result
     */
    uint32_t table[385] = {0};
    UINTList *keys = uintlist_create();

    for (size_t i = 0; i < string->length; i++)
    {
        uint32_t index = hash(string->chars[i].code);
        if (table[index] == 0 && index != 384)
        {
            uintlist_add(keys, index);
        }
        table[index]++;
    }

    double entropy = 0;

    for (size_t i = 0; i < keys->size; i++)
    {
        uint32_t index = uintlist_get(keys, i);
        double freq = 1.0 * table[index] / string->length;
        if (freq > 0)
        {
            entropy += freq * log2(freq);
        }
    }

    uintlist_destruct(keys);
    utf_destruct(string);
    PyBuffer_Release(&raw);

    return PyFloat_FromDouble(-1 * entropy);
}


static PyMethodDef ModuleMethods[] = {
    {"shannon_ch", shannon_ch, METH_VARARGS,
     "Calculate Shannon entropy for word."},
    {NULL, NULL, 0, NULL}        /* Sentinel */
};

static struct PyModuleDef nlptools = {
    PyModuleDef_HEAD_INIT,
    "nlptools", /* name of module */
    NULL,       /* module documentation, may be NULL */
    -1,         /* size of per-interpreter state of the module,
                 or -1 if the module keeps state in global variables. */
    ModuleMethods
};

PyMODINIT_FUNC
PyInit_nlptools(void)
{
    return PyModule_Create(&nlptools);
}