#include <lua5.4/lua.h>
#include <lua5.4/lauxlib.h>

#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <iso646.h>
#include <string.h>
#include <sys/signal.h>
#include <sys/types.h>
#include <sys/shm.h>

#include "chain/chain.h"

int asprintf(char **strp, const char *fmt, ...);

#define EVENT_MAX __UINT16_MAX__

typedef struct {
    pid_t list[EVENT_MAX];
    size_t nmemb;
} pidlist_t;

typedef struct {
    pidlist_t *get;
} holder_t;

typedef struct s_node {
    struct s_node *next;
    int index;
} node_t;

typedef struct {
    node_t *head;
} event_t;


static holder_t *binder(void)
{
    static holder_t list = { .get = NULL };

    return &list;
}

static bool *notif(void)
{
    static bool notif = false;

    return &notif;
}

static void sighandler(int sig)
{
    if (sig != SIGUSR1)
        return;
    *notif() = true;
}

static bool node_new(void *node, va_list *ap)
{
    node_t *self = node;

    self->index = va_arg(*ap, int);
    return 1;
}

static void binder_release(int id)
{
    if (shmdt(binder()->get) == -1)
        perror("shmdt");
    if (shmctl(id, IPC_RMID, NULL) == -1)
        perror("shmctl");
}

static int binder_create(int *id)
{
    *id = shmget(IPC_PRIVATE, sizeof(pidlist_t), IPC_CREAT | 0666);
    if (*id == -1)
        return -1;
    binder()->get = shmat(*id, NULL, 0);
    if (binder()->get == ((void *)-1))
        return -1;
    memset(binder()->get->list, 0, sizeof(pid_t) * EVENT_MAX);
    binder()->get->nmemb = 0;
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
    for (size_t n = 0; n < binder()->get->nmemb; ++n)
        (binder()->get->list[n]) and printf("Releasing %d\n",
        binder()->get->list[n]) and (kill(binder()->get->list[n], SIGTERM));
    binder_release(id);
    return -1;
}

static int event_once(lua_State *L)
{
    event_t *event = lua_touserdata(L, -2);
    pid_t pid = 0;
    pidlist_t *b = binder()->get;
    int index = b->nmemb;

    if (event == NULL)
        return luaL_error(L, "once: Can't get userdata.");
    if (signal(SIGUSR1, sighandler) == SIG_ERR)
        return luaL_error(L, "once: Can't catch signal.");
    chain_push(&event->head, sizeof(node_t), node_new, index);
    pid = fork();
    if (pid == -1)
        return luaL_error(L, "once: Can't fork.");
    if (pid) {
        b->list[b->nmemb++] = pid;
        return 0;
    }
    printf("Waiting for event on %d...\n", getpid());
    while (not *notif());
    b->list[index] = 0;
    lua_call(L, 0, 0);
    _exit(0);
}

static int event_fire(lua_State *L)
{
    event_t *event = lua_touserdata(L, -1);
    pid_t pid = 0;

    if (event == NULL)
        return luaL_error(L, "fire: Can't get userdata.");
    if (event->head == NULL) {
        lua_pushboolean(L, false);
        return 1;
    }
    for (node_t *it = event->head; it != NULL; it = it->next) {
        pid = binder()->get->list[it->index];
        pid and kill(pid, SIGUSR1);
    }
    lua_pushboolean(L, true);
    return 1;
}

static int event_display(lua_State *L)
{
    event_t *event = lua_touserdata(L, -1);
    char *str = "Super event";

    if (event == NULL)
        return luaL_error(L, "display: Can't get userdata");
    // asprintf(&str, "{ index = %d, pid = %d }",
    //     event->index,
    //     binder()->get->list[event->index]
    // );
    lua_pushstring(L, str);
    // free(str);
    return 1;
}

static int event_new(lua_State *L)
{
    event_t *event = lua_newuserdata(L, sizeof(event_t));

    if (event == NULL)
        return luaL_error(L, "Can't create userdata");
    event->head = NULL;
    lua_newtable(L);
    lua_pushstring(L, "__index");
    lua_newtable(L);
    lua_pushstring(L, "once");
    lua_pushcfunction(L, event_once);
    lua_settable(L, -3);
    lua_pushstring(L, "fire");
    lua_pushcfunction(L, event_fire);
    lua_settable(L, -3);
    lua_settable(L, -3);
    lua_pushstring(L, "__tostring");
    lua_pushcfunction(L, event_display);
    lua_settable(L, -3);
    lua_setmetatable(L, -2);
    return 1;
}

int luaopen_events(lua_State *L)
{
    if (binder_init(L) == -1)
        _exit(0);
    lua_newtable(L);
    lua_pushstring(L, "new");
    lua_pushcfunction(L, event_new);
    lua_settable(L, -3);
    return 1;
}