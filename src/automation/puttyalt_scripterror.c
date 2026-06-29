/* puttyalt_scripterror.c - Format and track script execution errors.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stdio.h>
#include <string.h>
#define SCE_MAX 16
typedef struct { int line; char message[80]; } SceError;
typedef struct { SceError errors[SCE_MAX]; int n; } ScriptErrors;
void sce_init(ScriptErrors *e) { if (e) e->n = 0; }
int sce_add(ScriptErrors *e, int line, const char *message) {
    if (!e || e->n >= SCE_MAX || !message) return -1;
    e->errors[e->n].line = line;
    strncpy(e->errors[e->n].message, message, 79); e->errors[e->n].message[79] = 0;
    e->n++;
    return 0;
}
int sce_format(const ScriptErrors *e, int idx, char *out, int outlen) {
    if (!e || idx < 0 || idx >= e->n || !out) return -1;
    return snprintf(out, outlen, "line %d: %s", e->errors[idx].line, e->errors[idx].message);
}
int sce_count(const ScriptErrors *e) { return e ? e->n : 0; }
