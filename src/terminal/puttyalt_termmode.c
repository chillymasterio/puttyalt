/* puttyalt_termmode.c - Terminal mode flags (ANSI/VT52/autowrap/origin). */
#include <string.h>
#define TM_AUTOWRAP    (1u << 0)
#define TM_ORIGIN      (1u << 1)
#define TM_INSERT      (1u << 2)
#define TM_APP_CURSOR  (1u << 3)
#define TM_APP_KEYPAD  (1u << 4)
#define TM_REVERSE     (1u << 5)
#define TM_VT52        (1u << 6)
#define TM_LINEFEED    (1u << 7)
typedef struct {
    unsigned int flags;
    unsigned int saved_flags;
} TermMode;
void termmode_init(TermMode *t) {
    if (!t) return;
    memset(t, 0, sizeof(*t));
    t->flags = TM_AUTOWRAP;
}
void termmode_set(TermMode *t, unsigned int flag) {
    if (t) t->flags |= flag;
}
void termmode_clear(TermMode *t, unsigned int flag) {
    if (t) t->flags &= ~flag;
}
int termmode_is_set(const TermMode *t, unsigned int flag) {
    return (t && (t->flags & flag)) ? 1 : 0;
}
void termmode_save(TermMode *t) {
    if (t) t->saved_flags = t->flags;
}
void termmode_restore(TermMode *t) {
    if (t) t->flags = t->saved_flags;
}
int termmode_decset(TermMode *t, int param, int set) {
    if (!t) return -1;
    unsigned int flag = 0;
    switch (param) {
        case 1: flag = TM_APP_CURSOR; break;
        case 6: flag = TM_ORIGIN; break;
        case 7: flag = TM_AUTOWRAP; break;
        default: return -1;
    }
    if (set) t->flags |= flag;
    else t->flags &= ~flag;
    return 0;
}
