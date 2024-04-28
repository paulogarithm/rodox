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
    pid_t *list;
} pidlist_t;

static pidlist_t *singleton_pids(void)
{
    static pidlist_t list = {0};

    return &list;
}

static int event_push(lua_State *L)
{
    lua_Integer n = 0;

    if (not lua_isinteger(L, -1))
        return luaL_error(L, "No integer");
    n = lua_tointeger(L, -1);
    singleton_pids()->list[0] = n;
    printf("%d\n", singleton_pids()->list[0]);
    return 0;
}

static int code_binder(lua_State *L)
{
    pid_t pid = 0;
    int id = shmget(IPC_PRIVATE, sizeof(int) * 2, IPC_CREAT | 0666);

    if (id == -1)
        return luaL_error(L, "Can't get shared mem");
    singleton_pids()->list = shmat(id, NULL, 0);
    if (singleton_pids()->list == ((void *)-1))
        return luaL_error(L, "Can't get the data");
    memset(singleton_pids()->list, 0, sizeof(int) * 2);
    pid = fork();
    if (pid == -1)
        return luaL_error(L, "Can't fork");
    if (pid == 0)
        return 1;
    printf("Binding the parent...\n");
    getchar();
    printf("%d\n", singleton_pids()->list[0]);
    return -1;
}

int luaopen_events(lua_State *L)
{
    if (code_binder(L) == -1)
        _exit(0);
    lua_newtable(L);
    lua_pushstring(L, "push");
    lua_pushcfunction(L, event_push);
    lua_settable(L, -3);
    return 1;
}