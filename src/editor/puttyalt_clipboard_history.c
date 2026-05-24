#include "puttyalt_clipboard_history.h"
#include <string.h>
#include <stdlib.h>

int clipboard_history_init(Clipboard_historyState *s)
{
    memset(s, 0, sizeof(*s));
    s->enabled = 1;
    s->initialized = 1;
    return 0;
}

int clipboard_history_update(Clipboard_historyState *s)
{
    if (!s->enabled || !s->initialized) return -1;
    return 0;
}

void clipboard_history_destroy(Clipboard_historyState *s)
{
    if (s->data) { free(s->data); s->data = NULL; }
    memset(s, 0, sizeof(*s));
}
