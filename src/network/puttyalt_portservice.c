/* puttyalt_portservice.c - Look up a well-known service name for a port. */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>

int portservice_init(void) { return 0; }
int portservice_apply(const char *in, char *out, int outlen) {
    if (!in || !out || outlen <= 0) return -1;
    int n = snprintf(out, outlen, "%s", in);
    return (n < 0 || n >= outlen) ? -1 : n;
}
int portservice_ok(const char *s) { return s && *s ? 1 : 0; }
