#ifndef PUTTYALT_QUICKBAR_H
#define PUTTYALT_QUICKBAR_H

typedef struct {
    char input[512];
    int visible;
    int focused;
    int cursor_pos;
    char placeholder[128];
    char *history[32];
    int history_count;
    int history_pos;
} QuickBar;

void quickbar_init(QuickBar *qb);
void quickbar_destroy(QuickBar *qb);
void quickbar_show(QuickBar *qb);
void quickbar_hide(QuickBar *qb);
void quickbar_toggle(QuickBar *qb);
int  quickbar_parse(QuickBar *qb, char *host, int *port, char *user);
void quickbar_add_history(QuickBar *qb, const char *entry);
void quickbar_history_prev(QuickBar *qb);
void quickbar_history_next(QuickBar *qb);

#endif
