/* puttyalt_tagindex.c - Index sessions by tag for fast lookup.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#define TIX_MAX 128
#define TIX_TAGLEN 32
typedef struct { char tag[TIX_TAGLEN]; int session_ids[16]; int count; } TixEntry;
typedef struct { TixEntry e[TIX_MAX]; int n; } TixIndex;
void tix_init(TixIndex *x) { if (x) x->n = 0; }
static int tix_find(TixIndex *x, const char *tag) {
    for (int i = 0; i < x->n; i++) if (strcmp(x->e[i].tag, tag) == 0) return i;
    return -1;
}
int tix_add(TixIndex *x, const char *tag, int session_id) {
    if (!x || !tag) return -1;
    int i = tix_find(x, tag);
    if (i < 0) {
        if (x->n >= TIX_MAX) return -1;
        i = x->n++;
        strncpy(x->e[i].tag, tag, TIX_TAGLEN - 1);
        x->e[i].tag[TIX_TAGLEN - 1] = 0;
        x->e[i].count = 0;
    }
    if (x->e[i].count >= 16) return -1;
    x->e[i].session_ids[x->e[i].count++] = session_id;
    return 0;
}
int tix_lookup(TixIndex *x, const char *tag, int *out, int outcap) {
    int i = tix_find(x, tag);
    if (i < 0) return 0;
    int n = x->e[i].count < outcap ? x->e[i].count : outcap;
    for (int k = 0; k < n; k++) out[k] = x->e[i].session_ids[k];
    return n;
}
