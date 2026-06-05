/* puttyalt_backupsched.c - Config backup scheduler with rotation policy. */
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#define BS_MAX 16
typedef struct { uint64_t ts; uint32_t hash; int slot; } bs_backup;
typedef struct { bs_backup b[BS_MAX]; int n; int interval_ms; int keep; uint64_t last_backup; } BackupSched;
void backupsched_init(BackupSched *b, int interval_ms, int keep) {
    if(!b) return; memset(b,0,sizeof(*b)); b->interval_ms=interval_ms>0?interval_ms:3600000; b->keep=keep>0&&keep<=BS_MAX?keep:8;
}
int backupsched_due(const BackupSched *b, uint64_t now_ms) {
    if(!b) return 0; return (b->last_backup==0 || (now_ms-b->last_backup)>=(uint64_t)b->interval_ms)?1:0;
}
int backupsched_record(BackupSched *b, uint32_t config_hash, uint64_t now_ms) {
    if(!b) return -1;
    /* skip if unchanged from latest */
    if (b->n>0 && b->b[b->n-1].hash==config_hash) { b->last_backup=now_ms; return 1; }
    if (b->n>=b->keep) { memmove(&b->b[0],&b->b[1],sizeof(bs_backup)*(b->n-1)); b->n--; }
    bs_backup *bk=&b->b[b->n]; bk->ts=now_ms; bk->hash=config_hash; bk->slot=b->n; b->n++;
    b->last_backup=now_ms; return 0;
}
int backupsched_count(const BackupSched *b) { return b?b->n:-1; }
uint64_t backupsched_latest_ts(const BackupSched *b) { return (b&&b->n>0)?b->b[b->n-1].ts:0; }
