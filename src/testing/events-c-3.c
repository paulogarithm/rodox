#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <iso646.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdarg.h>

#include "chain/chain.h"

typedef struct s_object {
    struct s_object *next;
    int pid;
} object_t;

static bool object_new(void *node, va_list *ap)
{
    object_t *obj = node;

    obj->pid = va_arg(*ap, int);
    return true;
}

static void object_close(void *node)
{
    object_t *obj = node;

    kill(obj->pid, SIGTERM);
}

static bool *event_singleton(void)
{
    static bool instance = false;

    return &instance;
}

static void sighandler(int sig)
{
    if (sig != SIGUSR1)
        return;
    *event_singleton() = true;
}

static int event_once(char const *str)
{
    __pid_t pid = 0;

    if (signal(SIGUSR1, sighandler) == SIG_ERR)
        return -1;
    pid = fork();
    if (pid == -1)
        return -1;
    if (not pid) {
        printf("Waiting for event on %d...\n", getpid());
        while (not *event_singleton());
        printf("%s\n", str);
        _exit(0);
    }
    return pid;
}

static int event_starter(void)
{
    char *str = NULL;
    int pid = fork();

    if (pid == -1)
        return -1;
    if (not pid) {
        printf("Main script in %d\n", getppid());
        return 0;
    }
    for (size_t s = 0; getline(&str, &s, stdin) > 0;)
        if (str != NULL)
            free(str);
    printf("End of code.\n");
    return 1;
}

int main(int argc, char const *argv[])
{
    int pids[32] = {0};
    char *str = NULL;
    size_t s = 0;
    object_t *head = NULL;

    if (not getpid())
        printf("Hello !\n");
    if (not event_starter()) {
        chain_push(&head, sizeof(object_t), object_new, event_once("Hello"));
        chain_push(&head, sizeof(object_t), object_new, event_once("world"));
        printf("Rest of the code %d\n", getpid());
        return 0;
    }
    printf("%p\n", head);
    chain_close(head, object_close);
    return 0;
}

