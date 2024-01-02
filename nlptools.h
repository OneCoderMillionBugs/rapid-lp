/**
 * Natural Language Processing Tools 1.0
 * 
 * This library is meant to be a set of highly efficient (compared to Python)
 * functions for language processing in russian and sometimes kazakh languages
 */

#ifndef NLPTOOLS_H
#define NLPTOOLS_H

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "utf8_string.h"
#include "uint_list.h"

/**
 * Calculates Shannon Entropy of a word
 * Entropy is sum of (freq * log2(freq)) calculated for each 
 * distinct character presented in the string,
 * where freq = number of appearances of a char / string length
 * 
 * Since initially we get negative values from log2, before return
 * answer (entropy) is multiplied by -1
 * 
 * Implementation details user should be aware of:
 * For performance and simplicity reasons, the function only supports
 * set of unicode ascii characters in range [U+0000, U+007F] and some 
 * cyrillic 
 */
static PyObject *shannon_ch(PyObject *self, PyObject *args);

#endif