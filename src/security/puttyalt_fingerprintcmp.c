/* puttyalt_fingerprintcmp.c - Compare key fingerprints safely.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#include <ctype.h>
/* Normalize a fingerprint (strip colons/spaces, lowercase) for comparison. */
int fpc_normalize(const char *fp, char *out, int outlen) {
    if (!fp || !out) return -1;
    int o = 0;
    for (const char *p = fp; *p && o < outlen - 1; p++) {
        if (*p == ':' || *p == ' ' || *p == '\n') continue;
        out[o++] = (char)tolower((unsigned char)*p);
    }
    out[o] = 0;
    return o;
}
/* Constant-time-ish comparison of normalized fingerprints. */
int fpc_equal(const char *a, const char *b) {
    char na[128], nb[128];
    if (fpc_normalize(a, na, sizeof na) < 0) return 0;
    if (fpc_normalize(b, nb, sizeof nb) < 0) return 0;
    if (strlen(na) != strlen(nb)) return 0;
    int diff = 0;
    for (int i = 0; na[i]; i++) diff |= na[i] ^ nb[i];
    return diff == 0;
}
