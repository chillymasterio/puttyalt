#include "puttyalt_proxy.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void proxy_init(ProxyChain *pc)
{
    memset(pc, 0, sizeof(*pc));
}

int proxy_add(ProxyChain *pc, ProxyType type, const char *host, int port)
{
    if (pc->chain_len >= PROXY_MAX_CHAIN) return -1;
    ProxyNode *n = &pc->chain[pc->chain_len];
    memset(n, 0, sizeof(*n));
    n->type = type;
    snprintf(n->host, PROXY_MAX_HOST, "%s", host);
    n->port = port;
    n->timeout_sec = 30;
    n->dns_through_proxy = 1;
    return pc->chain_len++;
}

int proxy_add_auth(ProxyChain *pc, int index, const char *user, const char *pass)
{
    if (index < 0 || index >= pc->chain_len) return -1;
    snprintf(pc->chain[index].username, 64, "%s", user);
    snprintf(pc->chain[index].password, 64, "%s", pass);
    return 0;
}

int proxy_remove(ProxyChain *pc, int index)
{
    if (index < 0 || index >= pc->chain_len) return -1;
    for (int i = index; i < pc->chain_len - 1; i++)
        pc->chain[i] = pc->chain[i + 1];
    pc->chain_len--;
    return 0;
}

int proxy_move_up(ProxyChain *pc, int index)
{
    if (index <= 0 || index >= pc->chain_len) return -1;
    ProxyNode tmp = pc->chain[index];
    pc->chain[index] = pc->chain[index - 1];
    pc->chain[index - 1] = tmp;
    return 0;
}

int proxy_move_down(ProxyChain *pc, int index)
{
    if (index < 0 || index >= pc->chain_len - 1) return -1;
    ProxyNode tmp = pc->chain[index];
    pc->chain[index] = pc->chain[index + 1];
    pc->chain[index + 1] = tmp;
    return 0;
}

ProxyNode *proxy_get_next(ProxyChain *pc)
{
    if (!pc->active || pc->chain_len == 0) return NULL;
    if (pc->rotate) {
        ProxyNode *n = &pc->chain[pc->current];
        pc->current = (pc->current + 1) % pc->chain_len;
        return n;
    }
    return &pc->chain[0];
}

int proxy_load(ProxyChain *pc, const char *path)
{
    FILE *f = fopen(path, "r");
    char line[512];
    ProxyNode *cur = NULL;
    if (!f) return -1;
    proxy_init(pc);
    while (fgets(line, sizeof(line), f)) {
        size_t len = strlen(line);
        while (len > 0 && (line[len-1] == '\n' || line[len-1] == '\r'))
            line[--len] = '\0';
        if (strcmp(line, "[proxy]") == 0) {
            if (pc->chain_len >= PROXY_MAX_CHAIN) break;
            cur = &pc->chain[pc->chain_len++];
            memset(cur, 0, sizeof(*cur));
            cur->timeout_sec = 30;
            continue;
        }
        if (!cur) continue;
        if (strncmp(line, "type=", 5) == 0) cur->type = atoi(line + 5);
        else if (strncmp(line, "host=", 5) == 0)
            snprintf(cur->host, PROXY_MAX_HOST, "%s", line + 5);
        else if (strncmp(line, "port=", 5) == 0) cur->port = atoi(line + 5);
        else if (strncmp(line, "user=", 5) == 0)
            snprintf(cur->username, 64, "%s", line + 5);
    }
    fclose(f);
    return 0;
}

int proxy_save(const ProxyChain *pc, const char *path)
{
    FILE *f = fopen(path, "w");
    if (!f) return -1;
    for (int i = 0; i < pc->chain_len; i++) {
        const ProxyNode *n = &pc->chain[i];
        fprintf(f, "[proxy]\ntype=%d\nhost=%s\nport=%d\nuser=%s\n\n",
                n->type, n->host, n->port, n->username);
    }
    fclose(f);
    return 0;
}

void proxy_enable(ProxyChain *pc, int enable)
{
    pc->active = enable;
}

void proxy_set_rotate(ProxyChain *pc, int rotate)
{
    pc->rotate = rotate;
    pc->current = 0;
}
