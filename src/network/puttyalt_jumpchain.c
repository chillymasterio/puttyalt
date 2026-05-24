#include "puttyalt_jumpchain.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void jump_init(JumpChain *jc) { memset(jc, 0, sizeof(*jc)); }

int jump_add_hop(JumpChain *jc, const char *host, int port, const char *user)
{
    if (jc->hop_count >= JUMP_MAX_HOPS) return -1;
    JumpHop *h = &jc->hops[jc->hop_count++];
    memset(h, 0, sizeof(*h));
    snprintf(h->host, sizeof(h->host), "%s", host);
    h->port = port > 0 ? port : 22;
    if (user) snprintf(h->user, sizeof(h->user), "%s", user);
    return 0;
}

int jump_remove_hop(JumpChain *jc, int index)
{
    if (index < 0 || index >= jc->hop_count) return -1;
    memmove(&jc->hops[index], &jc->hops[index + 1], (jc->hop_count - index - 1) * sizeof(JumpHop));
    jc->hop_count--;
    return 0;
}

int jump_parse_spec(JumpChain *jc, const char *spec)
{
    /* Format: "user1@host1:port1,user2@host2:port2,..." */
    char buf[2048];
    snprintf(buf, sizeof(buf), "%s", spec);
    jc->hop_count = 0;
    char *tok = strtok(buf, ",");
    while (tok && jc->hop_count < JUMP_MAX_HOPS) {
        char user[128] = "", host[256] = "";
        int port = 22;
        char *at = strchr(tok, '@');
        char *start = tok;
        if (at) { *at = '\0'; snprintf(user, sizeof(user), "%s", tok); start = at + 1; }
        char *colon = strchr(start, ':');
        if (colon) { *colon = '\0'; port = atoi(colon + 1); }
        snprintf(host, sizeof(host), "%s", start);
        if (host[0]) jump_add_hop(jc, host, port, user);
        tok = strtok(NULL, ",");
    }
    return jc->hop_count > 0 ? 0 : -1;
}

char *jump_to_string(JumpChain *jc, char *buf, int buflen)
{
    buf[0] = '\0';
    int pos = 0;
    for (int i = 0; i < jc->hop_count; i++) {
        JumpHop *h = &jc->hops[i];
        int n;
        if (h->user[0]) n = snprintf(buf + pos, buflen - pos, "%s%s@%s:%d", i ? " -> " : "", h->user, h->host, h->port);
        else n = snprintf(buf + pos, buflen - pos, "%s%s:%d", i ? " -> " : "", h->host, h->port);
        pos += n;
    }
    return buf;
}

int jump_validate(JumpChain *jc)
{
    if (jc->hop_count == 0) return -1;
    for (int i = 0; i < jc->hop_count; i++)
        if (jc->hops[i].host[0] == '\0' || jc->hops[i].port <= 0) return -1;
    return 0;
}
