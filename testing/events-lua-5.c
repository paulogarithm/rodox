#include <lua5.4/lua.h>
#include <lua5.4/lauxlib.h>

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <iso646.h>
#include <string.h>
#include <sys/signal.h>
#include <sys/types.h>
#include <sys/shm.h>

typedef struct {
    pid_t list[10];
    size_t nmemb;
} pidlist_t;

typedef struct {
    pidlist_t *binder;
} holder_t;

static holder_t *binder(void)
{
    static holder_t list = { .binder = NULL };

    return &list;
}

static int event_push(lua_State *L)
{
    lua_Integer n = 0;

    if (not lua_isinteger(L, -1))
        return luaL_error(L, "No integer");
    n = lua_tointeger(L, -1);
    binder()->binder->list[binder()->binder->nmemb] = n;
    printf("%d\n", binder()->binder->list[binder()->binder->nmemb]);
    ++binder()->binder->nmemb;
    return 0;
}

static void binder_release(int id)
{
    if (shmdt(binder()->binder) == -1)
        perror("shmdt");
    if (shmctl(id, IPC_RMID, NULL) == -1)
        perror("shmctl");
}

static int binder_create(int *id)
{
    *id = shmget(IPC_PRIVATE, sizeof(pidlist_t), IPC_CREAT | 0666);
    if (*id == -1)
        return -1;
    binder()->binder = shmat(*id, NULL, 0);
    if (binder()->binder == ((void *)-1))
        return -1;
    memset(binder()->binder->list, 0, sizeof(pid_t) * 10);
    binder()->binder->nmemb = 0;
    return 0;
}

static int binder_init(lua_State *L)
{
    pid_t pid = 0;
    int id = 0;

    if (binder_create(&id) == -1)
        return luaL_error(L, "Can't alloc shared mem");
    pid = fork();
    if (pid == -1)
        return luaL_error(L, "Can't fork");
    if (pid == 0)
        return 1;
    getchar();
    for (size_t n = 0; n < binder()->binder->nmemb; ++n)
        printf("Releasing %d\n", binder()->binder->list[n]);
    binder_release(id);
    return -1;
}

int luaopen_events(lua_State *L)
{
    if (binder_init(L) == -1)
        _exit(0);
    lua_newtable(L);
    lua_pushstring(L, "push");
    lua_pushcfunction(L, event_push);
    lua_settable(L, -3);
    return 1;
}