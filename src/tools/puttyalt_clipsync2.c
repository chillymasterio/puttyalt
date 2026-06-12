/* puttyalt_clipsync2.c - Clipboard sync ring with dedup + size cap. */
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#define CS_MAX 20
#define CS_LEN 512
typedef struct { char text[CS_LEN]; uint64_t ts; uint32_t hash; } cs_entry;
typedef struct { cs_entry e[CS_MAX]; int n; } ClipSync2;
static uint32_t cs_hash(const char *s) { uint32_t h=2166136261u; for(;*s;s++){h^=(uint8_t)*s;h*=16777619u;} return h; }
void clipsync2_init(ClipSync2 *c) { if(c) memset(c,0,sizeof(*c)); }
int clipsync2_push(ClipSync2 *c, const char *text, uint64_t now_ms) {
    if(!c||!text) return -1;
    uint32_t h=cs_hash(text);
    /* dedup: if top entry matches, skip */
    if (c->n>0 && c->e[c->n-1].hash==h) return 1;
    if (c->n>=CS_MAX) { memmove(&c->e[0],&c->e[1],sizeof(cs_entry)*(CS_MAX-1)); c->n--; }
    cs_entry *e=&c->e[c->n++]; snprintf(e->text,CS_LEN,"%s",text); e->ts=now_ms; e->hash=h; return 0;
}
const char *clipsync2_get(const ClipSync2 *c, int idx_from_top) {
    if(!c||idx_from_top<0||idx_from_top>=c->n) return 0;
    return c->e[c->n-1-idx_from_top].text;
}
int clipsync2_count(const ClipSync2 *c) { return c?c->n:-1; }
void clipsync2_clear(ClipSync2 *c) { if(c) c->n=0; }
