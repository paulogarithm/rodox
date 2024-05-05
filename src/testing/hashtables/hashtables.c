#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>

#include "chain/chain.h"
#include "hashtables.h"

/* Hash Table node */

static bool htnode_new(void *ptr, va_list *ap)
{
    htnode_t *e = ptr;

    if (e == NULL)
        return NULL;
    e->key = strdup(va_arg(*ap, void *));
    if (e->key == NULL) {
        free(e);
        return false;
    }
    e->value = strdup(va_arg(*ap, void *));
    if (e->value == NULL) {
        free(e->key);
        free(e);
        return false;
    }
    return true;
}

static void htnode_close(void *ptr)
{
    htnode_t *kv = ptr;

    free(kv->key);
    free(kv->value);
}

/* Hash Table */

ht_t *ht_new(size_t size)
{
    ht_t *ht = calloc(1, sizeof(ht_t));

    if (ht == NULL)
        return NULL;
    ht->size = size;
    ht->nmemb = 0;
    ht->items = calloc(size, sizeof(void *));
    if (ht->items == NULL) {
        free(ht);
        return NULL;
    }
    return ht;
}

void ht_close(ht_t *ht)
{
    for (size_t n = 0; n < ht->size; ++n)
        chain_close(ht->items[n], htnode_close);
    free(ht->items);
    free(ht);
}

int ht_hash(char const *s, int prime, int size)
{
    long int hash = 0;
    size_t len = strlen(s);

    for (size_t n = 0; n < len; ++n) {
        hash += (long int)pow(prime, len - (n + 1)) * s[n];
        hash %= size;
    }
    return (int)hash;
}

int ht_push(ht_t *ht, char const *key, char const *value)
{
    int index = ht_hash(key, HT_PRIME, ht->size);

    if (ht->items[index] != NULL)
        printf("Collision !\n");
    chain_push(&ht->items[index], sizeof(htnode_t), htnode_new, key, value);
    ++ht->nmemb;
    return 0;
}

char const *ht_seek(ht_t *ht, char const *key)
{
    int index = ht_hash(key, HT_PRIME, ht->size);

    for (htnode_t *it = ht->items[index]; it != NULL; it = it->next)
        if (!strcmp(it->key, key))
            return it->value;
    return NULL;
}

void ht_pop(ht_t *ht, char const *key)
{
    int index = ht_hash(key, HT_PRIME, ht->size);

    if (ht->items[index] == NULL)
        return;
    for (htnode_t *it = ht->items[index]; it != NULL; it = it->next)
        if (!strcmp(it->key, key)) {
            chain_pop(&ht->items[index], it, htnode_close);
            break;
        }
}
