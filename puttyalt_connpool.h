#ifndef PUTTYALT_CONNPOOL_H
#define PUTTYALT_CONNPOOL_H

#define POOL_MAX_CONN    16
#define POOL_HOST_LEN    256

typedef enum {
    CONN_FREE = 0,
    CONN_IN_USE,
    CONN_IDLE,
    CONN_CLOSING
} ConnState;

typedef struct PooledConnection {
    char host[POOL_HOST_LEN];
    int port;
    int socket_fd;
    ConnState state;
    unsigned long created_at;
    unsigned long last_used;
    int use_count;
} PooledConnection;

typedef struct ConnectionPool {
    PooledConnection conns[POOL_MAX_CONN];
    int count;
    int max_idle_sec;      /* max idle time before eviction */
    int max_per_host;      /* max connections per host */
} ConnectionPool;

void connpool_init(ConnectionPool *pool, int max_idle, int max_per_host);
int  connpool_acquire(ConnectionPool *pool, const char *host, int port);
int  connpool_release(ConnectionPool *pool, int index);
int  connpool_evict_idle(ConnectionPool *pool, unsigned long now);
int  connpool_count_host(const ConnectionPool *pool, const char *host);
void connpool_close_all(ConnectionPool *pool);

#endif
