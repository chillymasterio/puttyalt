/* puttyalt_keystore.c - SSH key store with metadata + agent integration. */
#include <string.h>
#include <stdio.h>
#define KS_MAX 32
#define KS_NAME 64
typedef struct { char name[KS_NAME]; char type[24]; char fingerprint[96]; int in_agent; int has_passphrase; int bits; } ks_key;
typedef struct { ks_key keys[KS_MAX]; int n; } KeyStore;
void keystore_init(KeyStore *k) { if(k) memset(k,0,sizeof(*k)); }
int keystore_add(KeyStore *k, const char *name, const char *type, const char *fp, int bits, int has_pass) {
    if(!k||k->n>=KS_MAX||!name) return -1;
    ks_key *e=&k->keys[k->n]; snprintf(e->name,KS_NAME,"%s",name); snprintf(e->type,24,"%s",type?type:"");
    snprintf(e->fingerprint,96,"%s",fp?fp:""); e->bits=bits; e->has_passphrase=has_pass?1:0; return k->n++;
}
int keystore_load_to_agent(KeyStore *k, int idx) {
    if(!k||idx<0||idx>=k->n) return -1; k->keys[idx].in_agent=1; return 0;
}
int keystore_find_by_fp(const KeyStore *k, const char *fp) {
    if(!k||!fp) return -1;
    for (int i=0;i<k->n;i++) if (strcmp(k->keys[i].fingerprint,fp)==0) return i;
    return -1;
}
int keystore_agent_count(const KeyStore *k) {
    if(!k) return -1; int n=0; for(int i=0;i<k->n;i++) if(k->keys[i].in_agent)n++; return n;
}
int keystore_count(const KeyStore *k) { return k?k->n:-1; }
