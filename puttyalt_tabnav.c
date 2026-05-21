#include "puttyalt_tabnav.h"
#include <string.h>

void tabnav_init(TabNav *tn)
{
    memset(tn, 0, sizeof(*tn));
    tn->mru_mode = 1;
}

void tabnav_set_active(TabNav *tn, int idx, int total)
{
    if (idx == tn->active && total == tn->total) return;
    tn->total = total;

    /* Add to MRU history */
    if (tn->history_count < TAB_NAV_MAX_HISTORY) {
        tn->history[tn->history_count++] = idx;
    } else {
        for (int i = 0; i < TAB_NAV_MAX_HISTORY - 1; i++)
            tn->history[i] = tn->history[i + 1];
        tn->history[TAB_NAV_MAX_HISTORY - 1] = idx;
    }
    tn->history_pos = tn->history_count - 1;
    tn->active = idx;
}

int tabnav_next(TabNav *tn)
{
    if (tn->total <= 1) return tn->active;
    tn->active = (tn->active + 1) % tn->total;
    return tn->active;
}

int tabnav_prev(TabNav *tn)
{
    if (tn->total <= 1) return tn->active;
    tn->active = (tn->active - 1 + tn->total) % tn->total;
    return tn->active;
}

int tabnav_mru_next(TabNav *tn)
{
    if (tn->history_count < 2) return tn->active;
    tn->history_pos--;
    if (tn->history_pos < 0) tn->history_pos = tn->history_count - 1;
    tn->active = tn->history[tn->history_pos];
    if (tn->active >= tn->total) tn->active = tn->total - 1;
    return tn->active;
}

int tabnav_goto(TabNav *tn, int idx)
{
    if (idx < 0 || idx >= tn->total) return -1;
    tn->active = idx;
    return idx;
}

int tabnav_close_current(TabNav *tn)
{
    if (tn->total <= 1) return 0;
    tn->total--;
    if (tn->active >= tn->total) tn->active = tn->total - 1;
    return tn->active;
}
