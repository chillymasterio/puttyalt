/* puttyalt_favorites2.c - Manage favorite session ranking.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#define FV_MAX 32
typedef struct { int session_id; int score; } FvEntry;
typedef struct { FvEntry e[FV_MAX]; int n; } Favorites2;
void fv2_init(Favorites2 *f) { if (f) f->n = 0; }
void fv2_use(Favorites2 *f, int session_id) {
    if (!f) return;
    for (int i = 0; i < f->n; i++) if (f->e[i].session_id == session_id) { f->e[i].score++; return; }
    if (f->n < FV_MAX) { f->e[f->n].session_id = session_id; f->e[f->n].score = 1; f->n++; }
}
/* Return top-N session ids by score into `out`. */
int fv2_top(Favorites2 *f, int *out, int n) {
    if (!f) return 0;
    /* selection sort copy */
    FvEntry tmp[FV_MAX];
    memcpy(tmp, f->e, sizeof(FvEntry) * f->n);
    int count = f->n < n ? f->n : n;
    for (int i = 0; i < count; i++) {
        int best = i;
        for (int j = i+1; j < f->n; j++) if (tmp[j].score > tmp[best].score) best = j;
        FvEntry t = tmp[i]; tmp[i] = tmp[best]; tmp[best] = t;
        out[i] = tmp[i].session_id;
    }
    return count;
}
