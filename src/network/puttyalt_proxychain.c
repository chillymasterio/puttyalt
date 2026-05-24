#include <string.h>
#include "puttyalt_proxychain.h"

void proxychain_init(ProxyChain *pc) { memset(pc, 0, sizeof(*pc)); }

int proxychain_add(ProxyChain *pc, ProxyType type, const char *host,
                   int port, const char *user, const char *pass)
{
    if (pc->num_hops >= PROXY_MAX_CHAIN) return -1;
    ProxyHop *h = &pc->hops[pc->num_hops];
    memset(h, 0, sizeof(*h));
    h->type = type;
    strncpy(h->host, host, PROXY_HOST_LEN - 1);
    h->port = port;
    if (user) strncpy(h->username, user, sizeof(h->username) - 1);
    if (pass) strncpy(h->password, pass, sizeof(h->password) - 1);
    pc->num_hops++;
    return 0;
}

int proxychain_remove(ProxyChain *pc, int index)
{
    if (index < 0 || index >= pc->num_hops) return -1;
    for (int i = index; i < pc->num_hops - 1; i++)
        pc->hops[i] = pc->hops[i + 1];
    pc->num_hops--;
    return 0;
}

void proxychain_clear(ProxyChain *pc) { pc->num_hops = 0; }
