/*
** EPITECH PROJECT, 2024
** repo
** File description:
** chain
*/

#ifndef CHAIN_H_
    #define CHAIN_H_

    #include <stdbool.h>
    #include <stdarg.h>

typedef struct {
    void *next;
} __node_t;

/**
 *  ### Push a value in your list ###
 * `headptr` - a pointer to your head pointer (&head).
 * `f` - your function to initialize your list. It gives you the node and a
 * va_list with your arguments.
 */
bool chain_push
(void *headptr, size_t nodesize, bool(*f)(void *self, va_list *list), ...);

/**
 * ### Free your chained list. ###
 * `head` - your head pointer.
 * `f` - function pointer to free stuff in your node. Set it to NULL if you
 * dont have nothing to free, it will just free the node.
 *
 * DO NOT FREE THE NODE IN YOUR FUNCTION
*/
void chain_close(void *head, void(*f)(void *));

/**
 * ### Pop a node. ###
 *
 * Pop a node from your chain list, using your node-closing function.
 *
 * `headptr` - a pointer to your head
 * `who` - a pointer to the node
 * `closef` - the function to close a node
*/
bool chain_pop(void *headptr, void const *who, void(*closef)(void *));

/**
 * Get the chain list length.
*/
size_t chain_len(void const *head);

#endif /* !CHAIN_H_ */
