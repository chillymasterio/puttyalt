#include <string.h>
#include "puttyalt_connpool.h"

void connpool_init(ConnectionPool *pool, int max_idle, int max_per_host)
{
    memset(pool, 0, sizeof(*pool));
    pool->max_idle_sec = max_idle > 0 ? max_idle : 300;
    pool->max_per_host = max_per_host > 0 ? max_per_host : 4;
}

int connpool_acquire(ConnectionPool *pool, const char *host, int port)
{
    /* Try to reuse an idle connection to the same host:port */
    for (int i = 0; i < pool->count; i++) {
        PooledConnection *c = &pool->conns[i];
        if (c->state == CONN_IDLE &&
            c->port == port &&
            strcmp(c->host, host) == 0) {
            c->state = CONN_IN_USE;
            c->use_count++;
            return i;
        }
    }

    /* Check per-host limit */
    if (connpool_count_host(pool, host) >= pool->max_per_host)
        return -1;

    /* Create new slot */
    if (pool->count >= POOL_MAX_CONN) return -1;
    PooledConnection *c = &pool->conns[pool->count];
    memset(c, 0, sizeof(*c));
    strncpy(c->host, host, POOL_HOST_LEN - 1);
    c->port = port;
    c->state = CONN_IN_USE;
    c->socket_fd = -1;
    c->use_count = 1;
    return pool->count++;
}

int connpool_release(ConnectionPool *pool, int index)
{
    if (index < 0 || index >= pool->count) return -1;
    pool->conns[index].state = CONN_IDLE;
    return 0;
}

int connpool_evict_idle(ConnectionPool *pool, unsigned long now)
{
    int evicted = 0;
    for (int i = pool->count - 1; i >= 0; i--) {
        PooledConnection *c = &pool->conns[i];
        if (c->state == CONN_IDLE &&
            now - c->last_used > (unsigned long)pool->max_idle_sec) {
            c->state = CONN_CLOSING;
            /* Shift down */
            for (int j = i; j < pool->count - 1; j++)
                pool->conns[j] = pool->conns[j + 1];
            pool->count--;
            evicted++;
        }
    }
    return evicted;
}

int connpool_count_host(const ConnectionPool *pool, const char *host)
{
    int count = 0;
    for (int i = 0; i < pool->count; i++)
        if (strcmp(pool->conns[i].host, host) == 0 &&
            pool->conns[i].state != CONN_CLOSING)
            count++;
    return count;
}

void connpool_close_all(ConnectionPool *pool)
{
    for (int i = 0; i < pool->count; i++)
        pool->conns[i].state = CONN_CLOSING;
    pool->count = 0;
}
