/* puttyalt_toastmgr.c - Toast manager with stacking + auto-dismiss. */
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#define TM_MAX 8
#define TM_TEXT 160
typedef struct {
    char text[TM_TEXT];
    int level;
    uint64_t created;
    int duration_ms;
} tm_toast;
typedef struct {
    tm_toast toasts[TM_MAX];
    int n;
    int max_visible;
} ToastMgr;
void toastmgr_init(ToastMgr *t, int max_visible) {
    if (!t) return;
    memset(t, 0, sizeof(*t));
    t->max_visible = max_visible > 0 ? max_visible : 3;
}
int toastmgr_show(ToastMgr *t, const char *text, int level, int duration_ms, uint64_t now_ms) {
    if (!t || !text) return -1;
    if (t->n >= TM_MAX) {
        memmove(&t->toasts[0], &t->toasts[1], sizeof(tm_toast) * (TM_MAX - 1));
        t->n--;
    }
    tm_toast *toast = &t->toasts[t->n++];
    snprintf(toast->text, TM_TEXT, "%s", text);
    toast->level = level;
    toast->created = now_ms;
    toast->duration_ms = duration_ms > 0 ? duration_ms : 4000;
    return 0;
}
int toastmgr_tick(ToastMgr *t, uint64_t now_ms) {
    if (!t) return -1;
    int removed = 0;
    int w = 0;
    for (int i = 0; i < t->n; i++) {
        if ((now_ms - t->toasts[i].created) < (uint64_t)t->toasts[i].duration_ms) {
            t->toasts[w++] = t->toasts[i];
        } else {
            removed++;
        }
    }
    t->n = w;
    return removed;
}
int toastmgr_visible(const ToastMgr *t) {
    if (!t) return -1;
    return t->n < t->max_visible ? t->n : t->max_visible;
}
