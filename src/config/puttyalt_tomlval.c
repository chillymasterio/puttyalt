/* puttyalt_tomlval.c - Parse scalar TOML values.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
enum { TVAL_STR, TVAL_INT, TVAL_FLOAT, TVAL_BOOL, TVAL_NONE };
typedef struct { int type; long ival; double fval; int bval; char sval[128]; } TomlVal;
int tval_parse(const char *s, TomlVal *out) {
    if (!s || !out) return -1;
    while (*s == ' ' || *s == '\t') s++;
    if (*s == '"' || *s == '\'') {
        char q = *s++; int o = 0;
        while (*s && *s != q && o < 127) out->sval[o++] = *s++;
        out->sval[o] = 0; out->type = TVAL_STR; return 0;
    }
    if (!strncmp(s, "true", 4)) { out->type = TVAL_BOOL; out->bval = 1; return 0; }
    if (!strncmp(s, "false", 5)) { out->type = TVAL_BOOL; out->bval = 0; return 0; }
    int isfloat = 0;
    for (const char *p = s; *p; p++) if (*p == '.' || *p == 'e' || *p == 'E') { isfloat = 1; break; }
    if (isfloat) { out->type = TVAL_FLOAT; out->fval = atof(s); return 0; }
    if (isdigit((unsigned char)*s) || *s == '-' || *s == '+') { out->type = TVAL_INT; out->ival = atol(s); return 0; }
    out->type = TVAL_NONE;
    return -1;
}
