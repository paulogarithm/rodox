#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>
#include <signal.h>
#include <sys/wait.h>

static int changed = 0;

static void handler(int signum)
{
    int *sin = eventSingleton();

    if (signum == SIGUSR1)
        changed = 1;
}

static int notif(int *ptr)
{
    while (!(*ptr));
    *ptr = 0;
    printf("notif !\n");
}

static int func(int *ptr)
{
    bool first = 1;
    bind:
    __pid_t pid = fork();

    if (pid == 0) {
        notif(ptr);
        _exit(0);
    }
    printf("pid: %d\n", pid);
    if (first) {
        printf("rest of the script...\n");
        first = 0;
    }
    wait(NULL);
    goto bind;
}

int main(int argc, char const *argv[])
{
    if (signal(SIGUSR1, handler) == SIG_ERR)
        return 1;
    func(&changed);
}
