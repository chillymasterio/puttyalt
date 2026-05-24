#ifndef PUTTYALT_PROXYCHAIN_H
#define PUTTYALT_PROXYCHAIN_H

#define PROXY_MAX_CHAIN  4
#define PROXY_HOST_LEN   256

typedef enum {
    PROXY_NONE = 0,
    PROXY_HTTP,
    PROXY_SOCKS4,
    PROXY_SOCKS5
} ProxyType;

typedef struct ProxyHop {
    ProxyType type;
    char host[PROXY_HOST_LEN];
    int port;
    char username[128];
    char password[128];
} ProxyHop;

typedef struct ProxyChain {
    ProxyHop hops[PROXY_MAX_CHAIN];
    int num_hops;
} ProxyChain;

void proxychain_init(ProxyChain *pc);
int  proxychain_add(ProxyChain *pc, ProxyType type, const char *host,
                    int port, const char *user, const char *pass);
int  proxychain_remove(ProxyChain *pc, int index);
void proxychain_clear(ProxyChain *pc);

#endif
