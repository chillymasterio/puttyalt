/* puttyalt_sgrparse.c - Parse SGR (color/attr) escape parameters.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
typedef struct { int fg, bg, bold, underline, reverse; } SgrState;
void sgr_reset(SgrState *s) { if (s) { s->fg = -1; s->bg = -1; s->bold = 0; s->underline = 0; s->reverse = 0; } }
void sgr_apply(SgrState *s, int code) {
    if (!s) return;
    if (code == 0) sgr_reset(s);
    else if (code == 1) s->bold = 1;
    else if (code == 4) s->underline = 1;
    else if (code == 7) s->reverse = 1;
    else if (code == 22) s->bold = 0;
    else if (code == 24) s->underline = 0;
    else if (code == 27) s->reverse = 0;
    else if (code >= 30 && code <= 37) s->fg = code - 30;
    else if (code == 39) s->fg = -1;
    else if (code >= 40 && code <= 47) s->bg = code - 40;
    else if (code == 49) s->bg = -1;
    else if (code >= 90 && code <= 97) s->fg = code - 90 + 8;
    else if (code >= 100 && code <= 107) s->bg = code - 100 + 8;
}
int sgr_parse(SgrState *s, const char *params) {
    if (!s || !params) return -1;
    int n = 0, code = 0, have = 0;
    for (const char *p = params; ; p++) {
        if (*p >= '0' && *p <= '9') { code = code * 10 + (*p - '0'); have = 1; }
        else { if (have) { sgr_apply(s, code); n++; } code = 0; have = 0; if (!*p) break; }
    }
    return n;
}
