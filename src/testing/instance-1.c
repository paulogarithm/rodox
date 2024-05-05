/**
 * Instance methods/fields
 * - parent
 * - name
 * - classname
 * - archivable (can be cloned)
 * 
 * - destroy()
 * - clone()
 * - isA()
 * - getChildren()
 * - findFirstChild()
 * - setAttribute()
 * - getAttribute()
*/

#include <lua5.4/lua.h>
#include <stdlib.h>

#include "chain/chain.h"
#include "hashtables/hashtables.h"

typedef struct s_property {
    struct s_property *next;
    char const *key;
    void *value;
} property_t;

typedef struct s_instance {
    struct s_instance *next;
    ht_t *properties;
} instance_t;

static instance_t *instance_new(char const *classname, instance_t *parent)
{
    instance_t *self = calloc(1, sizeof(instance_t));

    if (self == NULL)
        return NULL;
    self->properties = ht_new(30);
    if (self->properties == NULL) {
        free(self);
        return NULL;
    }
    return self;
}

int luaopen_instance(lua_State *L)
{
    if (code_binder(L) == -1)
        _exit(0);
    lua_newtable(L);
    lua_pushstring(L, "new");
    lua_pushcfunction(L, instance_new);
    lua_settable(L, -3);
    return 1;
}

