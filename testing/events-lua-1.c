#include <lua5.4/lua.h>
#include <lua5.4/lauxlib.h>
#include <lua5.4/lualib.h>

#include <unistd.h>
#include <sys/signal.h>
#include <stdbool.h>
#include <stddef.h>
#include <iso646.h>

static bool GLOBAL = false;

static void sighandler(int sig)
{
    if (sig != SIGUSR1)
        return;
    GLOBAL = true;
}

static int event_once(lua_State *L)
{
    __pid_t pid = 0;

    if (signal(SIGUSR1, sighandler) == SIG_ERR)
        return luaL_error(L, "Failed to catch signal.");
    pid = fork();
    if (pid == -1)
        return luaL_error(L, "Failed to fork.");
    if (pid == 0) {
        printf("Pushing event\n");
        lua_pushinteger(L, getppid());
        return true;
    }
    printf("Waiting for event on %d...\n", getpid());
    while (not GLOBAL);
    printf("Received event on %d\n", getpid());
    lua_call(L, 0, 0);
    _exit(0);
}

int luaopen_events(lua_State *L)
{
    lua_newtable(L);
    lua_pushstring(L, "once");
    lua_pushcfunction(L, event_once);
    lua_settable(L, -3);
    return 1;
}