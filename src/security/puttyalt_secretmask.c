/* puttyalt_secretmask.c - Mask secrets in structured data.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#include <ctype.h>
/* List of key names whose values should be masked. */
static int sm7_is_secret_key(const char *key) {
    static const char *keys[] = {"password","passwd","secret","token","key","apikey","auth"};
    for (unsigned i = 0; i < sizeof(keys)/sizeof(keys[0]); i++) {
        const char *k = keys[i], *p = key;
        /* case-insensitive substring */
        for (; *p; p++) {
            const char *a = p, *b = k;
            while (*a && *b && tolower((unsigned char)*a) == tolower((unsigned char)*b)) { a++; b++; }
            if (!*b) return 1;
        }
    }
    return 0;
}
/* Mask a "key=value" pair if the key is sensitive. */
int sm7_mask_kv(const char *key, const char *value, char *out, int outlen) {
    if (!key || !value || !out) return -1;
    if (sm7_is_secret_key(key)) {
        int n = 0;
        for (const char *p = key; *p && n < outlen-10; p++) out[n++] = *p;
        const char *mask = "=***";
        for (const char *m = mask; *m && n < outlen-1; m++) out[n++] = *m;
        out[n] = 0;
        return n;
    }
    int n = 0;
    for (const char *p = key; *p && n < outlen-2; p++) out[n++] = *p;
    if (n < outlen-1) out[n++] = '=';
    for (const char *p = value; *p && n < outlen-1; p++) out[n++] = *p;
    out[n] = 0;
    return n;
}
