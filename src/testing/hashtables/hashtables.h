#ifndef HASHTABLES_H_
    #define HASHTABLES_H_

    #include <stddef.h>

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

/* Remove a value with a key. */
void ht_pop(ht_t *ht, char const *key);

/* Seek for a value with a key. */
char const *ht_seek(ht_t *ht, char const *key);

/* Pushes a new value labelled to key. */
int ht_push(ht_t *ht, char const *key, char const *value);

/**
 * Hashes your string.
 * 
 * Use the size of your hashtable for `size`, and a prime like HT_PRIME for
 * the `prime` number.
*/
int ht_hash(char const *s, int prime, int size);

/* Create a new hashtable. */
ht_t *ht_new(size_t count);

/* Closes a hashtable. */
void ht_close(ht_t *ht);

#endif /* !HASHTABLES_H_ */
