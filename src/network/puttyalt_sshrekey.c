/* puttyalt_sshrekey.c - SSH rekey policy (by bytes/time, RFC 4253). */
#include <string.h>
#include <stdint.h>
typedef struct { uint64_t bytes_since; uint64_t max_bytes; uint64_t last_rekey_ms; int max_time_sec; int rekey_count; int pending; } SshRekey;
void sshrekey_init(SshRekey *r, uint64_t max_bytes, int max_time_sec, uint64_t now_ms) {
    if(!r) return; memset(r,0,sizeof(*r));
    r->max_bytes = max_bytes>0 ? max_bytes : (1ULL<<30); /* 1 GiB default */
    r->max_time_sec = max_time_sec>0 ? max_time_sec : 3600;
    r->last_rekey_ms = now_ms;
}
void sshrekey_account(SshRekey *r, uint64_t bytes) { if(r) r->bytes_since += bytes; }
int sshrekey_needed(const SshRekey *r, uint64_t now_ms) {
    if(!r) return 0;
    if (r->bytes_since >= r->max_bytes) return 1;
    if ((now_ms - r->last_rekey_ms) >= (uint64_t)r->max_time_sec * 1000) return 1;
    return 0;
}
int sshrekey_begin(SshRekey *r) { if(!r) return -1; r->pending=1; return 0; }
int sshrekey_complete(SshRekey *r, uint64_t now_ms) {
    if(!r) return -1; r->bytes_since=0; r->last_rekey_ms=now_ms; r->rekey_count++; r->pending=0; return 0;
}
int sshrekey_count(const SshRekey *r) { return r?r->rekey_count:-1; }
