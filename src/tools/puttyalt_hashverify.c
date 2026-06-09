/* puttyalt_hashverify.c - File hash verification (compare expected vs computed). */
#include <string.h>
#include <stdio.h>
enum hv_algo { HV_MD5=0, HV_SHA1=1, HV_SHA256=2, HV_SHA512=3 };
typedef struct { int algo; char expected[130]; char computed[130]; int verified; } HashVerify;
void hashverify_init(HashVerify *h, int algo, const char *expected) {
    if(!h) return; memset(h,0,sizeof(*h)); h->algo=algo; snprintf(h->expected,130,"%s",expected?expected:"");
}
int hashverify_set_computed(HashVerify *h, const char *computed) {
    if(!h||!computed) return -1; snprintf(h->computed,130,"%s",computed);
    /* case-insensitive compare */
    const char *a=h->expected, *b=h->computed;
    while (*a && *b) {
        char ca=*a, cb=*b; if(ca>='A'&&ca<='Z')ca+=32; if(cb>='A'&&cb<='Z')cb+=32;
        if (ca!=cb) { h->verified=0; return 0; } a++; b++;
    }
    h->verified=(*a==0 && *b==0)?1:0; return h->verified;
}
int hashverify_expected_len(int algo) {
    switch(algo){ case HV_MD5:return 32; case HV_SHA1:return 40; case HV_SHA256:return 64; case HV_SHA512:return 128; default:return 0; }
}
const char *hashverify_algo_name(int algo) {
    static const char *n[]={"MD5","SHA-1","SHA-256","SHA-512"};
    return (algo>=0&&algo<=HV_SHA512)?n[algo]:"?";
}
int hashverify_is_verified(const HashVerify *h) { return h?h->verified:-1; }
