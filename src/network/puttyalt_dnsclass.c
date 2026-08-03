/* puttyalt_dnsclass.c - Classify a hostname as loopback/private/public. */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>

int dnsclass_is_ok(const char *s) {
    if (!s || !*s) return 0;
    for (const char *p = s; *p; ++p) {
        unsigned char c = (unsigned char)*p;
        if (c < 0x20 || c == 0x7F) return 0;
    }
    return 1;
}
int dnsclass_parse(const char *in, char *out, int outlen) {
    if (!in || !out || outlen <= 0) return -1;
    if (!dnsclass_is_ok(in)) return -1;
    size_t n = strlen(in);
    if ((int)n >= outlen) n = outlen - 1;
    memcpy(out, in, n);
    out[n] = '\0';
    return (int)n;
}
int dnsclass_field_count(const char *s, char sep) {
    if (!s) return 0;
    int c = 1;
    for (const char *p = s; *p; ++p) if (*p == sep) c++;
    return c;
}
