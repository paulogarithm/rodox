/*
** EPITECH PROJECT, 2024
** rodox
** File description:
** shm-1
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iso646.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>

#define SHM_ERR ((void *)-1)

int main(void)
{
    int *var = NULL;
    int id = 0;
    pid_t pid = 0;

    id = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0666);
    if (id == -1)
        return EXIT_FAILURE;
    var = shmat(id, NULL, 0);
    if (var == SHM_ERR)
        return EXIT_FAILURE;
    pid = fork();
    if (pid == -1)
        return EXIT_FAILURE;
    if (not pid) {
        printf("child %d\n", *var);
        *var = 42;
        printf("child %d\n", *var);
    } else {
        printf("parent %d\n", *var);
        sleep(1);
        printf("parent %d\n", *var);
        wait(NULL);
        if (shmdt(var) == -1)
            perror("shmdt");
        if (shmctl(id, IPC_RMID, NULL) == -1)
            return EXIT_FAILURE;
    }
    return 0;
}
