/* puttyalt_keyseq.c - Key sequence to escape code mapper (terminfo-lite). */
#include <string.h>
#include <stdio.h>
#define KS_MAX 48
#define KS_NAME 24
#define KS_SEQ 16
typedef struct { char name[KS_NAME]; char seq[KS_SEQ]; int app_mode; } ks_key;
typedef struct { ks_key keys[KS_MAX]; int n; int app_cursor; } KeySeq;
void keyseq_init(KeySeq *k) { if(k) memset(k,0,sizeof(*k)); }
int keyseq_define(KeySeq *k, const char *name, const char *seq, int app_mode) {
    if(!k||k->n>=KS_MAX||!name||!seq) return -1;
    ks_key *key=&k->keys[k->n++]; snprintf(key->name,KS_NAME,"%s",name); snprintf(key->seq,KS_SEQ,"%s",seq); key->app_mode=app_mode;
    return 0;
}
const char *keyseq_lookup(const KeySeq *k, const char *name) {
    if(!k||!name) return 0;
    const char *fallback=0;
    for (int i=0;i<k->n;i++) if (strcmp(k->keys[i].name,name)==0) {
        if (k->keys[i].app_mode==k->app_cursor) return k->keys[i].seq;
        if (k->keys[i].app_mode<0) fallback=k->keys[i].seq;
        if (!fallback) fallback=k->keys[i].seq;
    }
    return fallback;
}
void keyseq_set_app_cursor(KeySeq *k, int on) { if(k) k->app_cursor=on?1:0; }
int keyseq_count(const KeySeq *k) { return k?k->n:-1; }
