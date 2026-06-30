/* puttyalt_keychord2.c - Format and parse key chord sequences.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#include <ctype.h>
typedef struct { int ctrl, alt, shift, super; char key; } KeyChord3;
int kc3_parse(const char *spec, KeyChord3 *out) {
    if (!spec || !out) return -1;
    memset(out, 0, sizeof *out);
    char tok[24]; int ti = 0;
    for (const char *p = spec; ; p++) {
        if (*p == '+' || *p == '-' || *p == 0) {
            tok[ti] = 0;
            if (!strcasecmp(tok, "ctrl") || !strcasecmp(tok, "c")) out->ctrl = 1;
            else if (!strcasecmp(tok, "alt") || !strcasecmp(tok, "m")) out->alt = 1;
            else if (!strcasecmp(tok, "shift") || !strcasecmp(tok, "s")) out->shift = 1;
            else if (!strcasecmp(tok, "super") || !strcasecmp(tok, "cmd")) out->super = 1;
            else if (ti == 1) out->key = (char)toupper((unsigned char)tok[0]);
            ti = 0;
            if (*p == 0) break;
        } else if (ti < 23) tok[ti++] = *p;
    }
    return out->key ? 0 : -1;
}
int kc3_format(const KeyChord3 *c, char *out, int outlen) {
    if (!c || !out) return -1;
    int o = 0;
    const char *mods[] = {"Ctrl+","Alt+","Shift+","Super+"};
    int flags[] = {c->ctrl, c->alt, c->shift, c->super};
    for (int i = 0; i < 4; i++) if (flags[i]) { const char *m=mods[i]; while(*m&&o<outlen-1)out[o++]=*m++; }
    if (o < outlen-1) out[o++] = c->key;
    out[o] = 0;
    return o;
}
