#ifndef PUTTYALT_JUMPCHAIN_H
#define PUTTYALT_JUMPCHAIN_H

#define JUMP_MAX_HOPS 8

typedef struct {
    char host[256];
    int port;
    char user[128];
    char keyfile[512];
    int proxy_type; /* 0=direct, 1=SOCKS5, 2=HTTP */
} JumpHop;

typedef struct {
    JumpHop hops[JUMP_MAX_HOPS];
    int hop_count;
    char name[128];
    int active;
    int current_hop;
} JumpChain;

void jump_init(JumpChain *jc);
int  jump_add_hop(JumpChain *jc, const char *host, int port, const char *user);
int  jump_remove_hop(JumpChain *jc, int index);
int  jump_parse_spec(JumpChain *jc, const char *spec);
char *jump_to_string(JumpChain *jc, char *buf, int buflen);
int  jump_validate(JumpChain *jc);

#endif
