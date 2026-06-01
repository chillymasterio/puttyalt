/* puttyalt_keyfmt.c - SSH public-key format detector + fingerprint label. */
#include <string.h>
#include <stdio.h>
enum keyfmt { KF_UNKNOWN=0, KF_RSA, KF_ED25519, KF_ECDSA, KF_DSA, KF_SK_ED25519, KF_SK_ECDSA };
int keyfmt_detect(const char *line) {
    if(!line) return KF_UNKNOWN;
    if (strstr(line,"ssh-ed25519")) return KF_ED25519;
    if (strstr(line,"ssh-rsa")) return KF_RSA;
    if (strstr(line,"ecdsa-sha2")) return KF_ECDSA;
    if (strstr(line,"ssh-dss")) return KF_DSA;
    if (strstr(line,"sk-ssh-ed25519")) return KF_SK_ED25519;
    if (strstr(line,"sk-ecdsa")) return KF_SK_ECDSA;
    return KF_UNKNOWN;
}
int keyfmt_name(int fmt, char *buf, int buflen) {
    static const char *n[]={"unknown","RSA","Ed25519","ECDSA","DSA","Ed25519-SK","ECDSA-SK"};
    if (fmt<0||fmt>KF_SK_ECDSA) return -1;
    return snprintf(buf,buflen,"%s",n[fmt]);
}
int keyfmt_is_legacy(int fmt) { return (fmt==KF_DSA||fmt==KF_RSA)?1:0; }
