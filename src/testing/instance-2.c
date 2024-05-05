#include <lua5.4/lua.h>
#include <lua5.4/lauxlib.h>
#include <stdlib.h>
#include <stdint.h>
#include <uuid/uuid.h>

#include "chain/chain.h"
#include "hashtables/hashtables.h"

typedef struct {
    lua_State *L;
} instance_t;

#define PROPERTY(L, k, def, f) do { \
        lua_pushstring(L, k); \
        lua_push##f(L, def); \
        lua_settable(L, -3); \
    } while (0)

static int instance_close(lua_State *L)
{
    instance_t *self = lua_touserdata(L, -1);
    
    if (self == NULL)
        return luaL_error(L, "Instance not found");
    lua_close(self->L);
    return 0;
}

static void instance_init(instance_t *self)
{
    union {
        __uint128_t n;
        uuid_t u;
    } u = { .n = 0 };

    self->L = luaL_newstate();
    lua_newtable(self->L);
    uuid_generate(u.u);
    PROPERTY(self->L, "Archivable", 0, boolean);
    PROPERTY(self->L, "ClassName", "Instance", string);
    PROPERTY(self->L, "Name", "Instance", string);
    PROPERTY(self->L, "UniqueId", u.n, integer);
}

static int instance_getter(lua_State *L)
{
    instance_t *self = lua_touserdata(L, -2);
    char const *str = lua_tostring(L, -1);

    if (self == NULL)
        return luaL_error(L, "Cant get Instance");
    lua_getfield(self->L, -1, str);
    lua_xmove(self->L, L, 1);
    return 1;
}

static int instance_new(lua_State *L)
{
    instance_t *self = lua_newuserdata(L, sizeof(instance_t));

    if (self == NULL)
        return luaL_error(L, "Cant create Instance");
    instance_init(self);
    lua_newtable(L);
    lua_pushstring(L, "__index");           // __index
    lua_pushcfunction(L, instance_getter);
    lua_settable(L, -3);                    // __index end
    lua_pushstring(L, "__gc");              // __gc 
    lua_pushcfunction(L, instance_close);
    lua_settable(L, -3);                    // __gc end
    lua_setmetatable(L, -2);
    return 1;
}

int luaopen_instance(lua_State *L)
{
    lua_newtable(L);
    lua_pushstring(L, "new");
    lua_pushcfunction(L, instance_new);
    lua_settable(L, -3);
    return 1;
}

