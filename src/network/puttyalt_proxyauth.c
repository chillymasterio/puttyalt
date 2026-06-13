/* puttyalt_proxyauth.c - Proxy authentication methods (basic/digest/ntlm). */
#include <string.h>
#include <stdio.h>
enum pa_method { PA_NONE = 0, PA_BASIC = 1, PA_DIGEST = 2, PA_NTLM = 3, PA_NEGOTIATE = 4 };
typedef struct {
    int method;
    char username[64];
    char realm[96];
    char nonce[64];
    int authenticated;
} ProxyAuth;
void proxyauth_init(ProxyAuth *p) {
    if (p) memset(p, 0, sizeof(*p));
}
int proxyauth_parse_challenge(ProxyAuth *p, const char *header) {
    if (!p || !header) return -1;
    if (strncmp(header, "Basic", 5) == 0) {
        p->method = PA_BASIC;
    } else if (strncmp(header, "Digest", 6) == 0) {
        p->method = PA_DIGEST;
        const char *realm = strstr(header, "realm=\"");
        if (realm) sscanf(realm + 7, "%95[^\"]", p->realm);
        const char *nonce = strstr(header, "nonce=\"");
        if (nonce) sscanf(nonce + 7, "%63[^\"]", p->nonce);
    } else if (strncmp(header, "NTLM", 4) == 0) {
        p->method = PA_NTLM;
    } else if (strncmp(header, "Negotiate", 9) == 0) {
        p->method = PA_NEGOTIATE;
    } else {
        return -1;
    }
    return 0;
}
int proxyauth_build_basic(const char *user, const char *pass, char *out, int outlen) {
    if (!user || !pass || !out) return -1;
    /* base64 of user:pass would go here; we emit the structure */
    return snprintf(out, outlen, "Proxy-Authorization: Basic %s:%s", user, pass);
}
int proxyauth_method(const ProxyAuth *p) {
    return p ? p->method : -1;
}
