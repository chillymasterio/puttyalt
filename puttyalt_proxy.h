#ifndef PUTTYALT_PROXY_H
#define PUTTYALT_PROXY_H

#define PROXY_MAX_CHAIN   8
#define PROXY_MAX_HOST    256

typedef enum {
    PROXY_NONE = 0,
    PROXY_SOCKS4,
    PROXY_SOCKS5,
    PROXY_HTTP,
    PROXY_HTTP_CONNECT
} ProxyType;

typedef struct {
    ProxyType type;
    char      host[PROXY_MAX_HOST];
    int       port;
    char      username[64];
    char      password[64];
    int       dns_through_proxy;
    int       timeout_sec;
} ProxyNode;

typedef struct {
    ProxyNode chain[PROXY_MAX_CHAIN];
    int       chain_len;
    int       active;
    int       rotate;       /* rotate through proxies */
    int       current;
} ProxyChain;

void proxy_init(ProxyChain *pc);
int  proxy_add(ProxyChain *pc, ProxyType type, const char *host, int port);
int  proxy_add_auth(ProxyChain *pc, int index, const char *user, const char *pass);
int  proxy_remove(ProxyChain *pc, int index);
int  proxy_move_up(ProxyChain *pc, int index);
int  proxy_move_down(ProxyChain *pc, int index);
ProxyNode *proxy_get_next(ProxyChain *pc);
int  proxy_load(ProxyChain *pc, const char *path);
int  proxy_save(const ProxyChain *pc, const char *path);
void proxy_enable(ProxyChain *pc, int enable);
void proxy_set_rotate(ProxyChain *pc, int rotate);

#endif
