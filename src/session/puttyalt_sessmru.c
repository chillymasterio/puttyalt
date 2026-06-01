/* puttyalt_sessmru.c - Most-recently-used session list with pinning. */
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#define MRU_MAX 64
#define MRU_NAME 96
typedef struct { char name[MRU_NAME]; uint64_t last_ms; int pinned; int uses; } mru_entry;
typedef struct { mru_entry e[MRU_MAX]; int n; } SessMru;
void sessmru_init(SessMru *m) { if(m) memset(m,0,sizeof(*m)); }
int sessmru_touch(SessMru *m, const char *name, uint64_t now_ms) {
    if(!m||!name) return -1;
    for (int i=0;i<m->n;i++) if (strcmp(m->e[i].name,name)==0) { m->e[i].last_ms=now_ms; m->e[i].uses++; return 0; }
    if (m->n>=MRU_MAX) {
        int lru=-1; uint64_t old=UINT64_MAX;
        for (int i=0;i<m->n;i++) if (!m->e[i].pinned && m->e[i].last_ms<old) { old=m->e[i].last_ms; lru=i; }
        if (lru<0) return -1;
        memmove(&m->e[lru],&m->e[lru+1],sizeof(mru_entry)*(m->n-lru-1)); m->n--;
    }
    mru_entry *e=&m->e[m->n++]; snprintf(e->name,MRU_NAME,"%s",name); e->last_ms=now_ms; e->uses=1; return 0;
}
int sessmru_pin(SessMru *m, const char *name, int pin) {
    if(!m||!name) return -1;
    for (int i=0;i<m->n;i++) if (strcmp(m->e[i].name,name)==0) { m->e[i].pinned=pin?1:0; return 0; }
    return -1;
}
int sessmru_top(const SessMru *m, int k, char *buf, int buflen) {
    if(!m||!buf) return -1;
    /* selection sort top-k by pinned, then last_ms */
    int idx[MRU_MAX]; for (int i=0;i<m->n;i++) idx[i]=i;
    for (int a=0;a<m->n;a++) for (int b=a+1;b<m->n;b++) {
        const mru_entry *x=&m->e[idx[a]], *y=&m->e[idx[b]];
        int better = (y->pinned>x->pinned) || (y->pinned==x->pinned && y->last_ms>x->last_ms);
        if (better) { int t=idx[a]; idx[a]=idx[b]; idx[b]=t; }
    }
    int pos=0, lim=k<m->n?k:m->n;
    for (int i=0;i<lim && pos<buflen;i++)
        pos+=snprintf(buf+pos,buflen-pos,"%s%s%s",m->e[idx[i]].pinned?"*":"",m->e[idx[i]].name,i<lim-1?"\n":"");
    return lim;
}
int sessmru_count(const SessMru *m) { return m?m->n:-1; }
