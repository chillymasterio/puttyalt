/* puttyalt_sessrecord.c - Tamper-evident session recording with hash chain. */
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#define SR_MAX 256
typedef struct { uint32_t prev_hash; uint32_t entry_hash; uint64_t ts; int data_len; } sr_block;
typedef struct { sr_block blocks[SR_MAX]; int n; uint32_t genesis; } SessRecord;
static uint32_t sr_hash(uint32_t prev, const char *data, int len, uint64_t ts) {
    uint32_t h=prev ^ (uint32_t)(ts & 0xFFFFFFFF);
    for (int i=0;i<len;i++) { h^=(uint8_t)data[i]; h*=16777619u; }
    return h;
}
void sessrecord_init(SessRecord *s, uint32_t seed) {
    if(!s) return; memset(s,0,sizeof(*s)); s->genesis=seed?seed:0x1234ABCD;
}
int sessrecord_append(SessRecord *s, const char *data, int len, uint64_t ts) {
    if(!s||!data||s->n>=SR_MAX) return -1;
    uint32_t prev = s->n==0 ? s->genesis : s->blocks[s->n-1].entry_hash;
    sr_block *b=&s->blocks[s->n++];
    b->prev_hash=prev; b->ts=ts; b->data_len=len;
    b->entry_hash=sr_hash(prev,data,len,ts);
    return s->n-1;
}
int sessrecord_verify(const SessRecord *s) {
    if(!s) return -1;
    uint32_t prev=s->genesis;
    for (int i=0;i<s->n;i++) {
        if (s->blocks[i].prev_hash != prev) return -1; /* chain broken */
        prev=s->blocks[i].entry_hash;
    }
    return 0;
}
uint32_t sessrecord_head_hash(const SessRecord *s) {
    return (s&&s->n>0)?s->blocks[s->n-1].entry_hash:(s?s->genesis:0);
}
int sessrecord_count(const SessRecord *s) { return s?s->n:-1; }
