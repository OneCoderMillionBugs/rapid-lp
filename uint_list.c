#include "uint_list.h"

UINTList *uintlist_create(void)
{
    UINTList *list = (UINTList *)malloc(sizeof(UINTList));
    list->size = 0;
    list->head = NULL;
    list->tail = NULL;

    return list;
}

void uintlist_add(UINTList *list, uint32_t value)
{
    if (list == NULL)
    {
        print_error("'uintlist_add' got null ptr");
        return;
    }

    Node *newnode = (Node *)malloc(sizeof(Node));
    newnode->next = NULL;
    newnode->value = value;

    if (list->size == 0)
    {
        list->head = newnode;
        list->tail = newnode;
    }
    else
    {
        list->tail->next = newnode;
        list->tail = newnode;
    }
    list->size++;
}

uint32_t uintlist_get(UINTList *list, size_t index)
{
    if (list == NULL)
    {
        print_error("'uintlist_get' got null ptr");
        return 0;
    }

    Node *current = list->head;
    size_t count = 0;

    while (count < index && current != NULL)
    {
        current = current->next;
        count++;
    }

    return current->value;
}

void uintlist_destruct(UINTList *list)
{
    if (list == NULL)
    {
        print_error("'uintlist_destruct' got null ptr");
        return;
    }

    Node *current = list->head;
    Node *prev = NULL;

    while (current != NULL)
    {
        prev = current;
        current = current->next;
        free(prev);
    }

    free(list);
}