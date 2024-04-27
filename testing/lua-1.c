#include <lua5.4/lua.h>
#include <lua5.4/lauxlib.h>

typedef struct {
    int num;
} foo_t;

static int foo_gc(lua_State *L)
{
    foo_t *data = lua_touserdata(L, -1);

    if (data == NULL)
        return luaL_error(L, "Can't get the userdata");
    printf("Destroyer\n");
    return 1;
}

static int foo_display(lua_State *L)
{
    foo_t *data = lua_touserdata(L, -1);

    if (data == NULL)
        return luaL_error(L, "Can't get the userdata");
    printf("%d\n", data->num);
    return 1;
}

static int foo_new(lua_State *L)
{
    foo_t *data = lua_newuserdata(L, sizeof(foo_t));

    if (data == NULL)
        return luaL_error(L, "Can't create userdata");
    data->num = 42;
    lua_newtable(L);
    lua_pushstring(L, "__index");
    lua_newtable(L);
    lua_pushstring(L, "display");
    lua_pushcfunction(L, foo_display);
    lua_settable(L, -3);
    lua_settable(L, -3);
    lua_pushstring(L, "__gc");
    lua_pushcfunction(L, foo_gc);
    lua_settable(L, -3);
    lua_setmetatable(L, -2);
    return 1;
}

int luaopen_foo(lua_State *L)
{
    lua_newtable(L);
    lua_pushstring(L, "new");
    lua_pushcfunction(L, foo_new);
    lua_settable(L, -3);
    return 1;
}