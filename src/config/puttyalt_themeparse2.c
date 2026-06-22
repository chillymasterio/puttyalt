/* puttyalt_themeparse2.c - Parse theme color definitions.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#define TP_MAX 24
typedef struct { char key[24]; uint32_t color; } ThemeColor;
typedef struct { ThemeColor c[TP_MAX]; int n; } Theme2;
void tp3_init(Theme2 *t) { if (t) t->n = 0; }
/* Parse "background=#1e1e1e" line. */
int tp3_line(Theme2 *t, const char *line) {
    if (!t || !line || t->n >= TP_MAX) return -1;
    const char *eq = strchr(line, '=');
    if (!eq) return -1;
    int kl = (int)(eq - line); if (kl >= 24) kl = 23;
    const char *v = eq + 1;
    if (*v == '#') v++;
    unsigned rgb;
    if (sscanf(v, "%x", &rgb) != 1) return -1;
    memcpy(t->c[t->n].key, line, kl); t->c[t->n].key[kl] = 0;
    t->c[t->n].color = rgb;
    t->n++;
    return 0;
}
int tp3_get(const Theme2 *t, const char *key, uint32_t *out) {
    if (!t || !key) return -1;
    for (int i = 0; i < t->n; i++) if (strcmp(t->c[i].key, key) == 0) { if (out) *out = t->c[i].color; return 0; }
    return -1;
}
