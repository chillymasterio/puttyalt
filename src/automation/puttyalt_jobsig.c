/* puttyalt_jobsig.c - Deterministic signature for a scheduled job. */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>

int jobsig_init(void) { return 0; }
int jobsig_apply(const char *in, char *out, int outlen) {
    if (!in || !out || outlen <= 0) return -1;
    int n = snprintf(out, outlen, "%s", in);
    return (n < 0 || n >= outlen) ? -1 : n;
}
int jobsig_ok(const char *s) { return s && *s ? 1 : 0; }
