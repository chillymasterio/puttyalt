/* puttyalt_ipv6fmt.c - Compress/expand IPv6 address text.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stdio.h>
#include <string.h>
/* Expand "::1" to full 8-group form. Returns 0 on success. */
int ip6_expand(const char *s, char *out, int outlen) {
    if (!s || !out) return -1;
    unsigned groups[8] = {0};
    const char *dc = strstr(s, "::");
    if (dc) {
        unsigned head[8], tail[8]; int nh = 0, nt = 0;
        char buf[64]; strncpy(buf, s, 63); buf[63] = 0;
        char *d = strstr(buf, "::"); *d = 0;
        char *p = buf;
        if (*p) { char *tok = p; for (char *q = p; ; q++) { if (*q == ':' || !*q) { char c=*q; *q=0; if (*tok) sscanf(tok,"%x",&head[nh++]); tok=q+1; if(!c) break; } } }
        char *t = d + 2;
        if (*t) { char *tok = t; for (char *q = t; ; q++) { if (*q == ':' || !*q) { char c=*q; *q=0; if (*tok) sscanf(tok,"%x",&tail[nt++]); tok=q+1; if(!c) break; } } }
        int idx = 0;
        for (int i = 0; i < nh; i++) groups[idx++] = head[i];
        idx = 8 - nt;
        for (int i = 0; i < nt; i++) groups[idx++] = tail[i];
    } else {
        if (sscanf(s, "%x:%x:%x:%x:%x:%x:%x:%x", &groups[0],&groups[1],&groups[2],&groups[3],&groups[4],&groups[5],&groups[6],&groups[7]) != 8) return -1;
    }
    return snprintf(out, outlen, "%04x:%04x:%04x:%04x:%04x:%04x:%04x:%04x",
        groups[0],groups[1],groups[2],groups[3],groups[4],groups[5],groups[6],groups[7]) > 0 ? 0 : -1;
}
