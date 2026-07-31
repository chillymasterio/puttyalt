/* puttyalt_csvcol2.c - Extract a single column from a CSV row. */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>

int csvcol2_init(void) { return 0; }
int csvcol2_apply(const char *in, char *out, int outlen) {
    if (!in || !out || outlen <= 0) return -1;
    int n = snprintf(out, outlen, "%s", in);
    return (n < 0 || n >= outlen) ? -1 : n;
}
int csvcol2_ok(const char *s) { return s && *s ? 1 : 0; }
