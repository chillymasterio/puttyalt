/* puttyalt_keymapparse.c - Parse keybinding spec strings.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#include <ctype.h>
typedef struct { int ctrl, alt, shift; int key; } KeyChord;
/* Parse "Ctrl+Shift+P" into modifiers + key code (ASCII upper). */
int kmp_parse(const char *spec, KeyChord *out) {
    if (!spec || !out) return -1;
    out->ctrl = out->alt = out->shift = 0; out->key = 0;
    char tok[32]; int ti = 0;
    for (const char *p = spec; ; p++) {
        if (*p == '+' || *p == 0) {
            tok[ti] = 0;
            if (ti > 0) {
                if (!strcasecmp(tok, "ctrl") || !strcasecmp(tok, "control")) out->ctrl = 1;
                else if (!strcasecmp(tok, "alt")) out->alt = 1;
                else if (!strcasecmp(tok, "shift")) out->shift = 1;
                else out->key = toupper((unsigned char)tok[0]);
            }
            ti = 0;
            if (*p == 0) break;
        } else if (ti < 31) tok[ti++] = *p;
    }
    return out->key ? 0 : -1;
}
int kmp_format(const KeyChord *c, char *out, int outlen) {
    if (!c || !out) return -1;
    int o = 0;
    if (c->ctrl)  { const char *m="Ctrl+";  while(*m&&o<outlen-1)out[o++]=*m++; }
    if (c->alt)   { const char *m="Alt+";   while(*m&&o<outlen-1)out[o++]=*m++; }
    if (c->shift) { const char *m="Shift+"; while(*m&&o<outlen-1)out[o++]=*m++; }
    if (o < outlen-1) out[o++] = (char)c->key;
    out[o] = 0;
    return o;
}
