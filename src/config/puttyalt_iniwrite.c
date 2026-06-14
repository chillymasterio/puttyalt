/* puttyalt_iniwrite.c - Serialize key/value pairs to INI text.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#include <stdio.h>
typedef struct { char buf[4096]; int len; } IniWriter;
void iniw_init(IniWriter *w) { if (w) { w->len = 0; w->buf[0] = 0; } }
int iniw_section(IniWriter *w, const char *name) {
    if (!w || !name) return -1;
    int n = snprintf(w->buf + w->len, sizeof(w->buf) - w->len, "[%s]\n", name);
    if (n < 0 || w->len + n >= (int)sizeof w->buf) return -1;
    w->len += n;
    return 0;
}
int iniw_kv(IniWriter *w, const char *k, const char *v) {
    if (!w || !k || !v) return -1;
    int n = snprintf(w->buf + w->len, sizeof(w->buf) - w->len, "%s=%s\n", k, v);
    if (n < 0 || w->len + n >= (int)sizeof w->buf) return -1;
    w->len += n;
    return 0;
}
int iniw_kv_int(IniWriter *w, const char *k, long v) {
    if (!w || !k) return -1;
    int n = snprintf(w->buf + w->len, sizeof(w->buf) - w->len, "%s=%ld\n", k, v);
    if (n < 0 || w->len + n >= (int)sizeof w->buf) return -1;
    w->len += n;
    return 0;
}
