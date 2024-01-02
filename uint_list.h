#ifndef UINTLIST_H
#define UINTLIST_H

#include <stdint.h>
#include "error.h"

struct node
{
    struct node *next;
    uint32_t value;
}
typedef Node;

struct uintlist
{
    struct node *head;
    struct node *tail;
    size_t size;
}
typedef UINTList;

/**
 * Create new UINTList on heap
 * Set each field to initial values
 */
UINTList *uintlist_create(void);

/**
 * Add an element to the end of a list
 * This function is of O(1) time
 */
void uintlist_add(UINTList *list, uint32_t value);

/**
 * Get nodes value at index n in linear (O(n)) time
 */
uint32_t uintlist_get(UINTList *list, size_t index);

/**
 * Destructs the list by freeing each nodes memory, 
 * and then the list itself
 * 
 * Highly recommended to set pointer on which this function was called to NULL!
 */
void uintlist_destruct(UINTList *list);

#endif