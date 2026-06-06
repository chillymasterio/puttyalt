/* puttyalt_sessquota.c - Per-session resource quota tracking (time/bytes/cmds). */
#include <string.h>
#include <stdint.h>
typedef struct { uint64_t max_bytes, used_bytes; int max_time_sec, used_time_sec; int max_cmds, used_cmds; int exceeded; } SessQuota;
void sessquota_init(SessQuota *q, uint64_t max_bytes, int max_time_sec, int max_cmds) {
    if(!q) return; memset(q,0,sizeof(*q)); q->max_bytes=max_bytes; q->max_time_sec=max_time_sec; q->max_cmds=max_cmds;
}
int sessquota_add_bytes(SessQuota *q, uint64_t bytes) {
    if(!q) return -1; q->used_bytes+=bytes;
    if (q->max_bytes>0 && q->used_bytes>q->max_bytes) { q->exceeded=1; return 1; }
    return 0;
}
int sessquota_add_time(SessQuota *q, int sec) {
    if(!q) return -1; q->used_time_sec+=sec;
    if (q->max_time_sec>0 && q->used_time_sec>q->max_time_sec) { q->exceeded=1; return 1; }
    return 0;
}
int sessquota_add_cmd(SessQuota *q) {
    if(!q) return -1; q->used_cmds++;
    if (q->max_cmds>0 && q->used_cmds>q->max_cmds) { q->exceeded=1; return 1; }
    return 0;
}
int sessquota_bytes_pct(const SessQuota *q) {
    if(!q||q->max_bytes==0) return 0; return (int)((q->used_bytes*100)/q->max_bytes);
}
int sessquota_is_exceeded(const SessQuota *q) { return q?q->exceeded:-1; }
