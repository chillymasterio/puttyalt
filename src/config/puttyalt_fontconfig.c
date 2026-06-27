/* puttyalt_fontconfig.c - Parse font specification strings.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
typedef struct { char family[64]; int size; int bold; int italic; } FontSpec;
/* Parse "Cascadia Code:size=12:bold". */
int fc2_parse(const char *spec, FontSpec *out) {
    if (!spec || !out) return -1;
    out->family[0] = 0; out->size = 12; out->bold = 0; out->italic = 0;
    const char *colon = strchr(spec, ':');
    int fl = colon ? (int)(colon - spec) : (int)strlen(spec);
    if (fl >= 64) fl = 63;
    memcpy(out->family, spec, fl); out->family[fl] = 0;
    const char *p = colon;
    while (p && *p) {
        p++;
        if (!strncmp(p, "size=", 5)) out->size = atoi(p + 5);
        else if (!strncmp(p, "bold", 4)) out->bold = 1;
        else if (!strncmp(p, "italic", 6)) out->italic = 1;
        p = strchr(p, ':');
    }
    return out->family[0] ? 0 : -1;
}
int fc2_format(const FontSpec *f, char *out, int outlen) {
    if (!f || !out) return -1;
    return snprintf(out, outlen, "%s:size=%d%s%s", f->family, f->size,
                    f->bold ? ":bold" : "", f->italic ? ":italic" : "");
}
