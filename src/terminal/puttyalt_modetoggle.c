/* puttyalt_modetoggle.c - DEC private mode set/reset tracking.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#define MTG_MAX 32
typedef struct { int mode; int enabled; } MtgEntry;
typedef struct { MtgEntry e[MTG_MAX]; int n; } ModeToggle;
void mtg_init(ModeToggle *m) { if (m) m->n = 0; }
void mtg_set(ModeToggle *m, int mode, int enabled) {
    if (!m) return;
    for (int i = 0; i < m->n; i++) if (m->e[i].mode == mode) { m->e[i].enabled = enabled; return; }
    if (m->n < MTG_MAX) { m->e[m->n].mode = mode; m->e[m->n].enabled = enabled; m->n++; }
}
int mtg_get(const ModeToggle *m, int mode) {
    if (!m) return 0;
    for (int i = 0; i < m->n; i++) if (m->e[i].mode == mode) return m->e[i].enabled;
    return 0;
}
/* Common modes: 1=DECCKM, 7=DECAWM, 25=cursor, 1049=alt-screen, 2004=bracketed paste */
int mtg_bracketed_paste(const ModeToggle *m) { return mtg_get(m, 2004); }
int mtg_app_cursor(const ModeToggle *m) { return mtg_get(m, 1); }
