/* puttyalt_argparse2.c - Parse command-line argument vectors.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#define AP_MAX 32
typedef struct { char flag[24]; char value[96]; int has_value; } ApArg;
typedef struct { ApArg args[AP_MAX]; int n; char positional[8][64]; int npos; } ArgParse;
void ap2_init(ArgParse *a) { if (a) { a->n = 0; a->npos = 0; } }
/* Parse "--flag=value" / "--flag" / "positional" style tokens. */
int ap2_add(ArgParse *a, const char *tok) {
    if (!a || !tok) return -1;
    if (tok[0] == '-' && tok[1] == '-') {
        if (a->n >= AP_MAX) return -1;
        ApArg *ar = &a->args[a->n];
        const char *eq = strchr(tok + 2, '=');
        if (eq) {
            int fl = (int)(eq - (tok+2)); if (fl >= 24) fl = 23;
            memcpy(ar->flag, tok+2, fl); ar->flag[fl] = 0;
            strncpy(ar->value, eq+1, 95); ar->value[95] = 0;
            ar->has_value = 1;
        } else {
            strncpy(ar->flag, tok+2, 23); ar->flag[23] = 0;
            ar->value[0] = 0; ar->has_value = 0;
        }
        a->n++;
    } else {
        if (a->npos >= 8) return -1;
        strncpy(a->positional[a->npos], tok, 63); a->positional[a->npos][63] = 0;
        a->npos++;
    }
    return 0;
}
const char *ap2_get(const ArgParse *a, const char *flag) {
    if (!a || !flag) return 0;
    for (int i = 0; i < a->n; i++) if (strcmp(a->args[i].flag, flag) == 0) return a->args[i].has_value ? a->args[i].value : "";
    return 0;
}
int ap2_has(const ArgParse *a, const char *flag) { return ap2_get(a, flag) != 0; }
