/**
 * Rodox chain list source code
*/

#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>
#include <iso646.h>

#include "chain.h"

bool chain_push
(void *headptr, size_t nodesize, bool(*newf)(void *, va_list *), ...)
{
    __node_t *node = calloc(1, nodesize);
    bool res = true;
    va_list ap = {0};

    va_start(ap, newf);
    if (node == NULL)
        return false;
    res = newf(node, &ap);
    node->next = *((void **)headptr);
    *((void **)headptr) = node;
    va_end(ap);
    return res;
}

void chain_close(void *head, void(*closef)(void *))
{
    __node_t *node = head;
    __node_t *next = NULL;

    while (node != NULL) {
        next = node->next;
        if (closef != NULL)
            closef(node);
        free(node);
        node = next;
    }
}

size_t chain_len(void const *head)
{
    size_t n = 0;

    for (__node_t const *it = head; it != NULL; it = it->next)
        ++n;
    return n;
}

bool chain_pop(void *headptr, void const *who, void(*closef)(void *))
{
    __node_t *node = *((void **)headptr);
    __node_t *prev = NULL;

    while ((node != NULL) and (node != who)) {
        prev = node;
        node = node->next;
    }
    if (node == NULL)
        return false;
    if (prev != NULL)
        prev->next = node->next;
    else
        *((void **)headptr) = node->next;
    if (closef != NULL)
        closef(node);
    free(node);
    return true;
}
