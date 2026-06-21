/* puttyalt_httphdr.c - Parse and store HTTP header sets.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#include <ctype.h>
#define HH_MAX 32
typedef struct { char name[48]; char value[160]; } HttpHeader;
typedef struct { HttpHeader h[HH_MAX]; int n; } HttpHeaders;
void hh2_init(HttpHeaders *h) { if (h) h->n = 0; }
int hh2_add(HttpHeaders *h, const char *line) {
    if (!h || !line || h->n >= HH_MAX) return -1;
    const char *colon = strchr(line, ':');
    if (!colon) return -1;
    int nl = (int)(colon - line); if (nl >= 48) nl = 47;
    HttpHeader *e = &h->h[h->n];
    for (int i = 0; i < nl; i++) e->name[i] = (char)tolower((unsigned char)line[i]);
    e->name[nl] = 0;
    const char *v = colon + 1;
    while (*v == ' ' || *v == '\t') v++;
    int o = 0;
    while (*v && *v != '\r' && *v != '\n' && o < 159) e->value[o++] = *v++;
    e->value[o] = 0;
    h->n++;
    return 0;
}
const char *hh2_get(const HttpHeaders *h, const char *name) {
    if (!h || !name) return 0;
    for (int i = 0; i < h->n; i++) if (strcasecmp(h->h[i].name, name) == 0) return h->h[i].value;
    return 0;
}
