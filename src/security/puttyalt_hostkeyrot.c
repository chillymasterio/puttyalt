/* puttyalt_hostkeyrot.c - Host key rotation tracking (RFC 8308 ext-info / new keys). */
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#define HKR_MAX 8
#define HKR_FP 64
typedef struct { char fingerprint[HKR_FP]; char keytype[24]; uint64_t first_seen; int active; } hkr_key;
typedef struct { hkr_key keys[HKR_MAX]; int n; char hostname[128]; } HostKeyRot;
void hostkeyrot_init(HostKeyRot *h, const char *hostname) {
    if(!h) return;
    memset(h,0,sizeof(*h)); snprintf(h->hostname,128,"%s",hostname?hostname:"");
}
int hostkeyrot_observe(HostKeyRot *h, const char *keytype, const char *fp, uint64_t now_ms) {
    if(!h||!fp) return -1;
    for (int i=0;i<h->n;i++) if (strcmp(h->keys[i].fingerprint,fp)==0) { h->keys[i].active=1; return 0; }
    if (h->n>=HKR_MAX) return -1;
    hkr_key *k=&h->keys[h->n++]; snprintf(k->fingerprint,HKR_FP,"%s",fp);
    snprintf(k->keytype,24,"%s",keytype?keytype:""); k->first_seen=now_ms; k->active=1;
    return h->n>1 ? 1 : 0; /* 1 = a new key beyond the first => rotation candidate */
}
int hostkeyrot_retire(HostKeyRot *h, const char *fp) {
    if(!h||!fp) return -1;
    for (int i=0;i<h->n;i++) if (strcmp(h->keys[i].fingerprint,fp)==0) { h->keys[i].active=0; return 0; }
    return -1;
}
int hostkeyrot_active_keys(const HostKeyRot *h) { if(!h) return -1; int n=0; for(int i=0;i<h->n;i++) if(h->keys[i].active)n++; return n; }
