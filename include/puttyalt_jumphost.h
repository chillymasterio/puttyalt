/*
 * puttyalt_jumphost.h: Jump host (bastion) connection chaining.
 *
 * Connect through one or more intermediate SSH servers to reach
 * the target host, similar to ssh -J / ProxyJump.
 */

#ifndef PUTTYALT_JUMPHOST_H
#define PUTTYALT_JUMPHOST_H

#define MAX_JUMP_HOPS 8

typedef struct JumpHop {
    char hostname[256];
    int  port;
    char username[128];
} JumpHop;

typedef struct JumpConfig {
    JumpHop hops[MAX_JUMP_HOPS];
    int     count;
    int     enabled;
} JumpConfig;

void jumphost_init(JumpConfig *jc);
int  jumphost_add_hop(JumpConfig *jc, const char *host, int port, const char *user);
int  jumphost_remove_hop(JumpConfig *jc, int index);
int  jumphost_parse_spec(JumpConfig *jc, const char *spec);

#endif /* PUTTYALT_JUMPHOST_H */
