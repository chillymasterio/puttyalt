/* puttyalt_titlestack.c - Window title save/restore stack (XTPUSHTITLE).
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#define TSK_DEPTH 8
#define TSK_LEN 128
typedef struct { char titles[TSK_DEPTH][TSK_LEN]; int sp; char current[TSK_LEN]; } TitleStack;
void tsk_init(TitleStack *t) { if (t) { t->sp = 0; t->current[0] = 0; } }
void tsk_set(TitleStack *t, const char *title) {
    if (!t || !title) return;
    size_t tl = strlen(title); if (tl >= TSK_LEN) tl = TSK_LEN - 1;
    memcpy(t->current, title, tl); t->current[tl] = 0;
}
int tsk_push(TitleStack *t) {
    if (!t || t->sp >= TSK_DEPTH) return -1;
    size_t pl = strlen(t->current); if (pl >= TSK_LEN) pl = TSK_LEN - 1;
    memcpy(t->titles[t->sp], t->current, pl); t->titles[t->sp][pl] = 0;
    t->sp++;
    return 0;
}
int tsk_pop(TitleStack *t) {
    if (!t || t->sp <= 0) return -1;
    t->sp--;
    size_t pl = strlen(t->titles[t->sp]); if (pl >= TSK_LEN) pl = TSK_LEN - 1;
    memcpy(t->current, t->titles[t->sp], pl); t->current[pl] = 0;
    return 0;
}
const char *tsk_current(const TitleStack *t) { return t ? t->current : ""; }
