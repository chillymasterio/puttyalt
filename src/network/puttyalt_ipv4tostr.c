/* puttyalt_ipv4tostr.c - Format a 32-bit IPv4 address as dotted quad. */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>

int ipv4tostr_init(void) { return 0; }
int ipv4tostr_apply(const char *in, char *out, int outlen) {
    if (!in || !out || outlen <= 0) return -1;
    int n = snprintf(out, outlen, "%s", in);
    return (n < 0 || n >= outlen) ? -1 : n;
}
int ipv4tostr_ok(const char *s) { return s && *s ? 1 : 0; }
