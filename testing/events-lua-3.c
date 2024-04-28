#include <lua5.4/lua.h>
#include <lua5.4/lauxlib.h>

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <iso646.h>
#include <sys/signal.h>
#include <sys/types.h>

int asprintf(char **strp, const char *fmt, ...);

typedef struct {
    int pid;
} event_t;

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

static int event_close(lua_State *L)
{
    event_t *event = lua_touserdata(L, -1);

    if (event == NULL)
        return luaL_error(L, "close: Can't get userdata");
    printf("Destroyer\n");
    // if (event->pid and kill(event->pid, SIGTERM) == -1)
    //     lua_warning(L, "Can't kill %d", event->pid);
    return 1;
}

static int event_display(lua_State *L)
{
    event_t *event = lua_touserdata(L, -1);
    char *str = NULL;

    if (event == NULL)
        return luaL_error(L, "display: Can't get userdata");
    asprintf(&str, "{ pid = %d }", event->pid);
    lua_pushstring(L, str);
    free(str);
    return 1;
}

static int event_once(lua_State *L)
{
    event_t *event = lua_touserdata(L, -2);

    if (event == NULL)
        return luaL_error(L, "once: Can't get userdata");
    if (signal(SIGUSR1, sighandler) == SIG_ERR)
        return -1;
    event->pid = fork();
    if (event->pid == -1)
        return -1;
    if (event->pid)
        return 0;
    printf("Waiting for event on %d...\n", getpid());
    while (not *event_singleton());
    lua_call(L, 0, 0);
    _exit(0);
}

static int event_new(lua_State *L)
{
    event_t *event = lua_newuserdata(L, sizeof(event_t));

    if (event == NULL)
        return luaL_error(L, "Can't create userdata");
    event->pid = 42;
    lua_newtable(L);
    lua_pushstring(L, "__index");       // __index
    lua_newtable(L);
    lua_pushstring(L, "once");
    lua_pushcfunction(L, event_once);
    lua_settable(L, -3);
    lua_settable(L, -3);                // not __index
    lua_pushstring(L, "__gc");
    lua_pushcfunction(L, event_close);
    lua_settable(L, -3);
    lua_pushstring(L, "__tostring");
    lua_pushcfunction(L, event_display);
    lua_settable(L, -3);
    lua_setmetatable(L, -2);
    return 1;
}

static int code_binder(lua_State *L)
{
    pid_t pid = fork();

    if (pid == -1)
        return luaL_error(L, "Can't fork");
    if (pid == 0)
        return 1;
    printf("Binding the parent...\n");
    getchar();
    lua_pushboolean(L, 1);
    return -1;
}

int luaopen_events(lua_State *L)
{
    if (code_binder(L) == -1)
        _exit(0);
    lua_newtable(L);
    lua_pushstring(L, "new");
    lua_pushcfunction(L, event_new);
    lua_settable(L, -3);
    return 1;
}