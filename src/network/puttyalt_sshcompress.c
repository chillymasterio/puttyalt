/* puttyalt_sshcompress.c - SSH compression negotiation + ratio tracking. */
#include <string.h>
#include <stdio.h>
#include <stdint.h>
enum sc_algo { SC_NONE=0, SC_ZLIB=1, SC_ZLIB_OPENSSH=2 };
typedef struct { int algo; int delayed; uint64_t raw_bytes; uint64_t compressed_bytes; int authenticated; } SshCompress;
void sshcompress_init(SshCompress *c) { if(c) memset(c,0,sizeof(*c)); }
int sshcompress_negotiate(SshCompress *c, const char *client_csv, const char *server_csv) {
    if(!c||!client_csv||!server_csv) return -1;
    if (strstr(client_csv,"zlib@openssh.com") && strstr(server_csv,"zlib@openssh.com")) { c->algo=SC_ZLIB_OPENSSH; c->delayed=1; }
    else if (strstr(client_csv,"zlib") && strstr(server_csv,"zlib")) { c->algo=SC_ZLIB; }
    else c->algo=SC_NONE;
    return c->algo;
}
int sshcompress_active(const SshCompress *c) {
    if(!c) return 0;
    if (c->algo==SC_NONE) return 0;
    if (c->delayed && !c->authenticated) return 0; /* delayed comp waits for auth */
    return 1;
}
void sshcompress_account(SshCompress *c, uint64_t raw, uint64_t compressed) {
    if(!c) return; c->raw_bytes+=raw; c->compressed_bytes+=compressed;
}
int sshcompress_ratio_pct(const SshCompress *c) {
    if(!c||c->raw_bytes==0) return 100;
    return (int)((c->compressed_bytes*100)/c->raw_bytes);
}
void sshcompress_set_authenticated(SshCompress *c) { if(c) c->authenticated=1; }
