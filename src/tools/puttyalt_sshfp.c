/* puttyalt_sshfp.c - SSHFP DNS record verification (RFC 4255). */
#include <string.h>
#include <stdio.h>
enum sfp_algo { SFP_RSA=1, SFP_DSA=2, SFP_ECDSA=3, SFP_ED25519=4 };
enum sfp_hash { SFP_SHA1=1, SFP_SHA256=2 };
typedef struct { int algo; int hash_type; char fingerprint[128]; } SshFp;
int sshfp_parse(const char *record, SshFp *out) {
    if(!record||!out) return -1;
    memset(out,0,sizeof(*out));
    if (sscanf(record,"%d %d %127s",&out->algo,&out->hash_type,out->fingerprint)==3) return 0;
    return -1;
}
int sshfp_matches(const SshFp *fp, int algo, const char *presented_hash) {
    if(!fp||!presented_hash) return 0;
    if (fp->algo!=algo) return 0;
    /* case-insensitive hex compare */
    const char *a=fp->fingerprint, *b=presented_hash;
    while (*a && *b) {
        char ca=*a, cb=*b; if(ca>='A'&&ca<='Z')ca+=32; if(cb>='A'&&cb<='Z')cb+=32;
        if (ca!=cb) return 0;
        a++; b++;
    }
    return (*a==0 && *b==0)?1:0;
}
const char *sshfp_algo_name(int algo) {
    switch(algo){ case SFP_RSA:return "RSA"; case SFP_DSA:return "DSA"; case SFP_ECDSA:return "ECDSA"; case SFP_ED25519:return "Ed25519"; default:return "?"; }
}
