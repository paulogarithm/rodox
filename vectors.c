/*
** EPITECH PROJECT, 2024
** c_vectors
** File description:
** vectors
*/

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct {
    size_t nmemb;
    size_t total;
    size_t size;
} vector_t;

void *vector_new(size_t size)
{
    size_t startnmemb = 4;
    vector_t *vec = calloc(1, sizeof(vector_t) + (startnmemb * size));

    if (vec == NULL)
        return NULL;
    vec->nmemb = 0;
    vec->total = startnmemb;
    vec->size = size;
    return (char *)vec + sizeof(vector_t);
}

int vector_push(void *p, void *elem)
{
    vector_t *vec = (vector_t *)((char *)(*(void **)p) - sizeof(vector_t));
    char *where = NULL;

    if (vec->nmemb >= vec->total) {
        vec->total *= 2;
        printf("Realloc with %ld elements\n", vec->total);
        vec = realloc(vec, sizeof(vector_t) + (vec->total * vec->size));
    }
    where = ((char *)vec + sizeof(vector_t)) + vec->size * vec->nmemb;
    memcpy(where, elem, vec->size);
    ++vec->nmemb;
    *(void **)p = (char *)vec + sizeof(vector_t);
    return 0;
}

void vector_free(void *v)
{
    free(((char *)v) - sizeof(vector_t));
}

int main(int argc, char const *argv[])
{
    int *v = vector_new(sizeof(int));
    int n = 3;

    vector_push(&v, &n);
    vector_push(&v, &n);
    vector_push(&v, &n);
    vector_push(&v, &n);
    vector_push(&v, &n);
    printf("%d\n", v[4]);
    vector_free(v);
    return 0;
}

