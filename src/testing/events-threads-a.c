#include <lua5.4/lua.h>
#include <lua5.4/lauxlib.h>

#include <semaphore.h>
#include <pthread.h>
#include <iso646.h>
#include <stdlib.h>

typedef struct {
    sem_t sem;
    pthread_t thread;
    lua_State *L;
} event_t;

static void *func(void *ptr)
{
    event_t *event = ptr;

    sem_wait(&event->sem);
    return NULL;
}

static int event_once(lua_State *L)
{
    event_t *event = lua_newuserdata(L, sizeof(event_t));

    if (event == NULL)
        return luaL_error(L, "Can't create userdata");
    sem_init(&event->sem, 0, 0);
    event->thread = 0;
    event->L = L;
    pthread_create(&event->thread, NULL, func, event);
    printf("%d\n", lua_gettop(L));
    return 1;
}

int luaopen_events(lua_State *L)
{
    lua_newtable(L);
    lua_pushstring(L, "once");
    lua_pushcfunction(L, event_once);
    lua_settable(L, -3);
    return 1;
}