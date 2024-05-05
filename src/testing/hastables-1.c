#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

typedef struct {
    char *key;
    char *value;
} htkv_t;

typedef struct {
    size_t size;
    size_t nmemb;
    htkv_t **items;
} ht_t;

#define HT_PRIME 5351

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

static htkv_t *htkv_new(char const *k, char const *v)
{
    htkv_t *e = calloc(1, sizeof(htkv_t));

    if (e == NULL)
        return NULL;
    e->key = strdup(k);
    if (e->key == NULL) {
        free(e);
        return NULL;
    }
    e->value = strdup(v);
    if (e->value == NULL) {
        free(e);
        free(e->key);
        return NULL;
    }
    return e;
}

static void htkv_close(htkv_t *kv)
{
    free(kv->key);
    free(kv->value);
    free(kv);
}

static void ht_close(ht_t *ht)
{
    for (size_t n = 0; n < ht->size; ++n)
        if (ht->items[n] != NULL)
            htkv_close(ht->items[n]);
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
    htkv_t *kv = htkv_new(key, value);
    int index = 0;

    if (kv == NULL)
        return -1;
    index = ht_hash(kv->key, HT_PRIME, ht->size);
    if (ht->items[index] != NULL) {
        printf("Collision !\n");
        htkv_close(ht->items[index]);
        --ht->nmemb;
    }
    ht->items[index] = kv;
    ++ht->nmemb;
    return 0;
}

static char const *ht_seek(ht_t *ht, char const *key)
{
    int index = ht_hash(key, HT_PRIME, ht->size);

    return ht->items[index]->value;
}

static void ht_pop(ht_t *ht, char const *key)
{
    int index = ht_hash(key, HT_PRIME, ht->size);

    if (ht->items[index] == NULL)
        return;
    htkv_close(ht->items[index]);
    ht->items[index] = NULL;
}

int main(void)
{
    ht_t *ht = ht_new(30);

    ht_push(ht, "Paul", "Hello");
    printf("%s\n", ht_seek(ht, "Paul"));
    ht_pop(ht, "Paul");
    ht_close(ht);
    return 0;
}
