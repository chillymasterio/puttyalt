/* puttyalt_md5.c - Lightweight checksum (FNV-1a + simple digest, for cache keys). */
#include <string.h>
#include <stdio.h>
#include <stdint.h>
typedef struct { uint64_t h1, h2; uint64_t len; } Digest;
void digest_init(Digest *d) { if(d){ d->h1=1469598103934665603ULL; d->h2=1099511628211ULL; d->len=0; } }
void digest_update(Digest *d, const unsigned char *data, int len) {
    if(!d||!data) return;
    for (int i=0;i<len;i++) {
        d->h1 ^= data[i]; d->h1 *= 1099511628211ULL;
        d->h2 = (d->h2<<5) + d->h2 + data[i]; /* djb2 variant */
    }
    d->len += len;
}
int digest_hex(const Digest *d, char *out, int outlen) {
    if(!d||!out) return -1;
    return snprintf(out,outlen,"%016llx%016llx",(unsigned long long)d->h1,(unsigned long long)d->h2);
}
uint64_t digest_value(const Digest *d) { return d?(d->h1 ^ d->h2):0; }
int digest_string(const char *str, char *out, int outlen) {
    Digest d; digest_init(&d); digest_update(&d,(const unsigned char*)str,(int)strlen(str)); return digest_hex(&d,out,outlen);
}
