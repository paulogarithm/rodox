#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>

#include "chain/chain.h"

typedef struct s_htnode {
    struct s_htnode *next;
    char *key;
    char *value;
} htnode_t;

typedef struct {
    size_t size;
    size_t nmemb;
    htnode_t **items;
} ht_t;

#define HT_PRIME 5351

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

static ht_t *ht_new(size_t count)
{
    ht_t *ht = calloc(1, sizeof(ht_t));

    if (ht == NULL)
        return NULL;
    ht->size = count;
    ht->nmemb = 0;
    ht->items = calloc(count, sizeof(void *));
    if (ht->items == NULL) {
        free(ht);
        return NULL;
    }
    return ht;
}

static void ht_close(ht_t *ht)
{
    for (size_t n = 0; n < ht->size; ++n)
        chain_close(ht->items[n], htnode_close);
    free(ht->items);
    free(ht);
}

static int ht_hash(char const *s, int a, int m)
{
    long int hash = 0;
    size_t len = strlen(s);

    for (size_t n = 0; n < len; ++n) {
        hash += (long int)pow(a, len - (n + 1)) * s[n];
        hash %= m;
    }
    return (int)hash;
}

static int ht_push(ht_t *ht, char const *key, char const *value)
{
    int index = ht_hash(key, HT_PRIME, ht->size);

    if (ht->items[index] != NULL)
        printf("Collision !\n");
    chain_push(&ht->items[index], sizeof(htnode_t), htnode_new, key, value);
    ++ht->nmemb;
    return 0;
}

static char const *ht_seek(ht_t *ht, char const *key)
{
    int index = ht_hash(key, HT_PRIME, ht->size);

    for (htnode_t *it = ht->items[index]; it != NULL; it = it->next)
        if (!strcmp(it->key, key))
            return it->value;
    return NULL;
}

static void ht_pop(ht_t *ht, char const *key)
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

int main(void)
{
    ht_t *ht = ht_new(30);

    ht_push(ht, "Name", "Hello");
    ht_push(ht, "ClassName", "Hello");
    ht_push(ht, "Archivable", "Hello");
    ht_push(ht, "Parent", "Hello");
    ht_push(ht, "UniqueId", "Hello");
    printf("%s\n", ht_seek(ht, "Name"));
    ht_close(ht);
    return 0;
}
