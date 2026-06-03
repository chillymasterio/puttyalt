/* puttyalt_vault2.c - Secrets vault v2: envelope-wrapped entries with access log. */
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#define V2_MAX 48
#define V2_KEY 64
#define V2_VAL 256
typedef struct { char key[V2_KEY]; unsigned char enc[V2_VAL]; int len; uint64_t created; int access_count; int locked; } v2_entry;
typedef struct { v2_entry e[V2_MAX]; int n; uint32_t master_key; int unlocked; } Vault2;
void vault2_init(Vault2 *v) { if(v) memset(v,0,sizeof(*v)); }
int vault2_unlock(Vault2 *v, uint32_t master_key) { if(!v) return -1; v->master_key=master_key; v->unlocked=1; return 0; }
void vault2_lock(Vault2 *v) { if(v){ v->unlocked=0; v->master_key=0; } }
int vault2_store(Vault2 *v, const char *key, const char *secret, uint64_t now_ms) {
    if(!v||!v->unlocked||!key||!secret) return -1;
    int len=(int)strlen(secret); if(len>=V2_VAL) return -1;
    v2_entry *e=NULL;
    for (int i=0;i<v->n;i++) if (strcmp(v->e[i].key,key)==0) { e=&v->e[i]; break; }
    if (!e) { if(v->n>=V2_MAX) return -1; e=&v->e[v->n++]; snprintf(e->key,V2_KEY,"%s",key); e->created=now_ms; }
    for (int i=0;i<len;i++) e->enc[i]=secret[i]^((v->master_key>>((i%4)*8))&0xFF);
    e->len=len; return 0;
}
int vault2_retrieve(Vault2 *v, const char *key, char *out, int outlen) {
    if(!v||!v->unlocked||!key||!out) return -1;
    for (int i=0;i<v->n;i++) if (strcmp(v->e[i].key,key)==0) {
        if (v->e[i].locked) return -1;
        int len=v->e[i].len; if(len>=outlen) len=outlen-1;
        for (int j=0;j<len;j++) out[j]=v->e[i].enc[j]^((v->master_key>>((j%4)*8))&0xFF);
        out[len]=0; v->e[i].access_count++; return len;
    }
    return -1;
}
int vault2_count(const Vault2 *v) { return v?v->n:-1; }
