/* puttyalt_yamlscalar.c - Parse YAML scalar values and types.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
enum { YS_NULL, YS_BOOL, YS_INT, YS_FLOAT, YS_STRING };
typedef struct { int type; long ival; int bval; char sval[128]; } YamlScalar;
int ys2_parse(const char *s, YamlScalar *out) {
    if (!s || !out) return -1;
    while (*s == ' ') s++;
    if (*s == 0 || !strcmp(s, "null") || !strcmp(s, "~")) { out->type = YS_NULL; return 0; }
    if (!strcmp(s, "true") || !strcmp(s, "yes")) { out->type = YS_BOOL; out->bval = 1; return 0; }
    if (!strcmp(s, "false") || !strcmp(s, "no")) { out->type = YS_BOOL; out->bval = 0; return 0; }
    char *end;
    long iv = strtol(s, &end, 10);
    if (*end == 0) { out->type = YS_INT; out->ival = iv; return 0; }
    int has_dot = 0;
    for (const char *p = s; *p; p++) if (*p == '.') has_dot = 1;
    if (has_dot) { double d = strtod(s, &end); if (*end == 0) { out->type = YS_FLOAT; out->ival = (long)d; return 0; } }
    out->type = YS_STRING;
    const char *v = s;
    if (*v == '"' || *v == '\'') v++;
    strncpy(out->sval, v, 127); out->sval[127] = 0;
    int l = (int)strlen(out->sval);
    if (l > 0 && (out->sval[l-1] == '"' || out->sval[l-1] == '\'')) out->sval[l-1] = 0;
    return 0;
}
