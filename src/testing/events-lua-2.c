#include <unistd.h>
#include <stdbool.h>

#include <lua5.4/lua.h>
#include <lua5.4/lauxlib.h>

typedef struct {
    __pid_t pid;
} event_t;

static int event_connect(lua_State *L)
{
    event_t *this = lua_touserdata(L, 1);

    if (this == NULL)
        return luaL_error(L, "can't get user data.");
    printf("connect with pid %d\n", this->pid);
    return 1;
}

static int event_disconnect(lua_State *L)
{
    printf("disconnecting.\n");
    return 1;
}

static int event_fire(lua_State *L)
{
    printf("fireing.\n");
    return 1;
}

static int event_new(lua_State *L)
{
    event_t *data = lua_newuserdata(L, sizeof(event_t));

    if (data == NULL)
        return luaL_error(L, "Can't create userdata");
    data->pid = 0;
    lua_newtable(L);
    lua_pushstring(L, "__index");
    lua_newtable(L);
    lua_pushstring(L, "connect");
    lua_pushcfunction(L, event_connect);
    lua_settable(L, -3);
    lua_pushstring(L, "fire");
    lua_pushcfunction(L, event_fire);
    lua_settable(L, -3);
    lua_pushstring(L, "disconnect");
    lua_pushcfunction(L, event_disconnect);
    lua_settable(L, -3);
    lua_settable(L, -3);
    lua_setmetatable(L, -2);
    return 1;
}

int luaopen_event(lua_State *L)
{
    lua_newtable(L);
    lua_pushstring(L, "new");
    lua_pushcfunction(L, event_new);
    lua_settable(L, -3);
    return 1;
}