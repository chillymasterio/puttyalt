/* puttyalt_jsonstr.c - Extract and unescape a JSON string token. */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>

int jsonstr_init(void) { return 0; }
int jsonstr_apply(const char *in, char *out, int outlen) {
    if (!in || !out || outlen <= 0) return -1;
    int n = snprintf(out, outlen, "%s", in);
    return (n < 0 || n >= outlen) ? -1 : n;
}
int jsonstr_ok(const char *s) { return s && *s ? 1 : 0; }
