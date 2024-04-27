#include <lua5.4/lua.h>
#include <lua5.4/lauxlib.h>

typedef struct {
    int number;
} adder_t;

static int adder_add(lua_State *L)
{
    adder_t *adder = lua_touserdata(L, 1);
    int toadd = 1;

    if (adder == NULL)
        return luaL_error(L, "Can't querry userdata");
    if (lua_isnumber(L, 2))
        toadd = luaL_checknumber(L, 2);
    adder->number += toadd;
    return 0;
}

static int adder_display(lua_State *L)
{
    adder_t *adder = lua_touserdata(L, 1);

    if (adder == NULL)
        return luaL_error(L, "Can't querry userdata");
    printf("%d\n", adder->number);
    return 0;
}

static int adder_new(lua_State *L)
{
    adder_t *data = lua_newuserdata(L, sizeof(adder_t));

    if (data == NULL)
        return luaL_error(L, "Can't create userdata");
    data->number = 0;
    lua_newtable(L);
    lua_pushstring(L, "__index");
    lua_newtable(L);
    lua_pushstring(L, "inc");
    lua_pushcfunction(L, adder_add);
    lua_settable(L, -3);
    lua_pushstring(L, "display");
    lua_pushcfunction(L, adder_display);
    lua_settable(L, -3);
    lua_settable(L, -3);
    lua_setmetatable(L, -2);
    return 1;
}

int luaopen_adder(lua_State *L)
{
    lua_newtable(L);
    lua_pushstring(L, "new");
    lua_pushcfunction(L, adder_new);
    lua_settable(L, -3);
    return 1;
}