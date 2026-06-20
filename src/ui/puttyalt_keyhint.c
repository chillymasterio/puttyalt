/* puttyalt_keyhint.c - Format keyboard shortcut hint bars.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stdio.h>
#include <string.h>
typedef struct { char buf[256]; int len; } KeyHints;
void kh2_init(KeyHints *k) { if (k) { k->buf[0] = 0; k->len = 0; } }
int kh2_add(KeyHints *k, const char *key, const char *action) {
    if (!k || !key || !action) return -1;
    int n = snprintf(k->buf + k->len, sizeof(k->buf) - k->len, "%s%s:%s",
                     k->len ? "  " : "", key, action);
    if (n < 0 || k->len + n >= (int)sizeof k->buf) return -1;
    k->len += n;
    return 0;
}
