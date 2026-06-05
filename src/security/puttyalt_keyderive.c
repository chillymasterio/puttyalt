/* puttyalt_keyderive.c - HKDF-style key derivation chain (expand/extract bookkeeping). */
#include <string.h>
#include <stdint.h>
#define KD_MAX_KEYS 16
#define KD_KEYLEN 32
typedef struct { unsigned char key[KD_KEYLEN]; int len; char label[24]; } kd_key;
typedef struct { kd_key keys[KD_MAX_KEYS]; int n; unsigned char prk[KD_KEYLEN]; int have_prk; } KeyDerive;
void keyderive_init(KeyDerive *k) { if(k) memset(k,0,sizeof(*k)); }
int keyderive_extract(KeyDerive *k, const unsigned char *salt, int saltlen, const unsigned char *ikm, int ikmlen) {
    if(!k||!ikm) return -1;
    /* simplified HMAC-extract: fold ikm with salt into PRK */
    memset(k->prk,0,KD_KEYLEN);
    for (int i=0;i<ikmlen;i++) k->prk[i%KD_KEYLEN]^=ikm[i];
    for (int i=0;i<saltlen;i++) k->prk[i%KD_KEYLEN]^=(salt[i]<<1);
    k->have_prk=1; return 0;
}
int keyderive_expand(KeyDerive *k, const char *label, int outlen) {
    if(!k||!k->have_prk||!label||k->n>=KD_MAX_KEYS||outlen>KD_KEYLEN) return -1;
    kd_key *kk=&k->keys[k->n]; kk->len=outlen;
    unsigned char ctr=1;
    for (int i=0;i<outlen;i++) {
        unsigned char v=k->prk[i%KD_KEYLEN] ^ (unsigned char)label[i%((int)strlen(label)?(int)strlen(label):1)] ^ ctr;
        kk->key[i]=v; if ((i+1)%KD_KEYLEN==0) ctr++;
    }
    for (int i=0;i<23 && label[i];i++) kk->label[i]=label[i];
    return k->n++;
}
const unsigned char *keyderive_get(const KeyDerive *k, const char *label, int *len) {
    if(!k||!label) return 0;
    for (int i=0;i<k->n;i++) if (strncmp(k->keys[i].label,label,23)==0) { if(len)*len=k->keys[i].len; return k->keys[i].key; }
    return 0;
}
int keyderive_count(const KeyDerive *k) { return k?k->n:-1; }
