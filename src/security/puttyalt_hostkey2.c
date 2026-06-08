/* puttyalt_hostkey2.c - Host key trust store v2 with key rotation acceptance. */
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#define HK_MAX 128
#define HK_HOST 96
#define HK_FP 96
enum hk_trust { HK_UNKNOWN=0, HK_TRUSTED=1, HK_CHANGED=2, HK_REVOKED=3 };
typedef struct { char host[HK_HOST]; char keytype[24]; char fingerprint[HK_FP]; int trust; uint64_t first_seen; } hk_entry;
typedef struct { hk_entry e[HK_MAX]; int n; } HostKey2;
void hostkey2_init(HostKey2 *h) { if(h) memset(h,0,sizeof(*h)); }
int hostkey2_check(HostKey2 *h, const char *host, const char *keytype, const char *fp, uint64_t now_ms) {
    if(!h||!host||!fp) return HK_UNKNOWN;
    for (int i=0;i<h->n;i++) if (strcmp(h->e[i].host,host)==0 && strcmp(h->e[i].keytype,keytype?keytype:"")==0) {
        if (h->e[i].trust==HK_REVOKED) return HK_REVOKED;
        if (strcmp(h->e[i].fingerprint,fp)==0) return HK_TRUSTED;
        return HK_CHANGED; /* same host+type, different key */
    }
    /* new host: record as unknown (caller decides to trust) */
    if (h->n<HK_MAX) {
        hk_entry *e=&h->e[h->n++]; snprintf(e->host,HK_HOST,"%s",host); snprintf(e->keytype,24,"%s",keytype?keytype:"");
        snprintf(e->fingerprint,HK_FP,"%s",fp); e->trust=HK_UNKNOWN; e->first_seen=now_ms;
    }
    return HK_UNKNOWN;
}
int hostkey2_trust(HostKey2 *h, const char *host, const char *fp) {
    if(!h||!host||!fp) return -1;
    for (int i=0;i<h->n;i++) if (strcmp(h->e[i].host,host)==0 && strcmp(h->e[i].fingerprint,fp)==0) { h->e[i].trust=HK_TRUSTED; return 0; }
    return -1;
}
int hostkey2_revoke(HostKey2 *h, const char *host) {
    if(!h||!host) return -1; int n=0;
    for (int i=0;i<h->n;i++) if (strcmp(h->e[i].host,host)==0) { h->e[i].trust=HK_REVOKED; n++; }
    return n;
}
int hostkey2_count(const HostKey2 *h) { return h?h->n:-1; }
