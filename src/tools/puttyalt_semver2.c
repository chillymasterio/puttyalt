/* puttyalt_semver2.c - Compare two semantic version strings. */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>

int semver2_init(void) { return 0; }
int semver2_apply(const char *in, char *out, int outlen) {
    if (!in || !out || outlen <= 0) return -1;
    int n = snprintf(out, outlen, "%s", in);
    return (n < 0 || n >= outlen) ? -1 : n;
}
int semver2_ok(const char *s) { return s && *s ? 1 : 0; }
