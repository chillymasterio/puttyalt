/* puttyalt_certsubject.c - Parse X.509 subject DN fields.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
/* Extract a field value (e.g. "CN") from a DN string "CN=host,O=org". */
int cs3_field(const char *dn, const char *key, char *out, int outlen) {
    if (!dn || !key || !out) return -1;
    int klen = (int)strlen(key);
    const char *p = dn;
    while (*p) {
        while (*p == ' ' || *p == ',') p++;
        if (strncmp(p, key, klen) == 0 && p[klen] == '=') {
            const char *v = p + klen + 1;
            int o = 0;
            while (*v && *v != ',' && o < outlen - 1) out[o++] = *v++;
            out[o] = 0;
            return o;
        }
        while (*p && *p != ',') p++;
    }
    return -1;
}
int cs3_has_field(const char *dn, const char *key) {
    char tmp[128];
    return cs3_field(dn, key, tmp, sizeof tmp) >= 0;
}
