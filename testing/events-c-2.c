#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <iso646.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>

void *vector_new(size_t size);
int vector_push(void *p, void *elem);

static bool GLOBAL = false;

static void sighandler(int sig)
{
    if (sig != SIGUSR1)
        return;
    GLOBAL = true;
}

typedef struct s_eventnode {
    struct s_eventnode *next;
    int pid;
} eventnode_t;

static int event_once(char const *str)
{
    __pid_t pid = 0;
    __pid_t child = 0;

    if (signal(SIGUSR1, sighandler) == SIG_ERR)
        return -1;
    pid = fork();
    if (pid == -1)
        return -1;
    if (pid == 0) {
        printf("Waiting for event on %d...\n", getpid());
        while (not GLOBAL);
        printf("%s\n", str);
        _exit(0);
    }
    return pid;
}

int main(int argc, char const *argv[])
{
    int pids[32] = {0};
    int size = 0;
    char *str = NULL;
    size_t s = 0;

    pids[size++] = event_once("Hello");
    pids[size++] = event_once("world");
    printf("Rest of the code %d\n", getpid());
    if (getline(&str, &s, stdin) == -1)
        return 1;
    for (int n = 0; n < size; ++n)
        kill(pids[n], SIGTERM);
    return 0;
}

