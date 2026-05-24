/*
 * puttyalt_jumphost.c: Jump host configuration parser.
 */

#include "puttyalt_jumphost.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void jumphost_init(JumpConfig *jc)
{
    memset(jc, 0, sizeof(*jc));
}

int jumphost_add_hop(JumpConfig *jc, const char *host, int port, const char *user)
{
    if (jc->count >= MAX_JUMP_HOPS)
        return -1;
    JumpHop *h = &jc->hops[jc->count];
    snprintf(h->hostname, sizeof(h->hostname), "%s", host);
    h->port = port > 0 ? port : 22;
    if (user)
        snprintf(h->username, sizeof(h->username), "%s", user);
    jc->enabled = 1;
    return jc->count++;
}

int jumphost_remove_hop(JumpConfig *jc, int index)
{
    if (index < 0 || index >= jc->count)
        return -1;
    for (int i = index; i < jc->count - 1; i++)
        jc->hops[i] = jc->hops[i + 1];
    jc->count--;
    if (jc->count == 0) jc->enabled = 0;
    return 0;
}

/* Parse spec like "user1@host1:port1,user2@host2:port2" */
int jumphost_parse_spec(JumpConfig *jc, const char *spec)
{
    char buf[2048];
    snprintf(buf, sizeof(buf), "%s", spec);
    jumphost_init(jc);

    char *saveptr;
    char *token = strtok_r(buf, ",", &saveptr);
    while (token && jc->count < MAX_JUMP_HOPS) {
        char *at = strchr(token, '@');
        char *user = NULL;
        char *hostport;

        if (at) {
            *at = '\0';
            user = token;
            hostport = at + 1;
        } else {
            hostport = token;
        }

        char *colon = strchr(hostport, ':');
        int port = 22;
        if (colon) {
            *colon = '\0';
            port = atoi(colon + 1);
        }

        jumphost_add_hop(jc, hostport, port, user);
        token = strtok_r(NULL, ",", &saveptr);
    }
    return jc->count;
}
