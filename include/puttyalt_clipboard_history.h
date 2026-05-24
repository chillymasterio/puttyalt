#ifndef PUTTYALT_CLIPBOARD_HISTORY_H
#define PUTTYALT_CLIPBOARD_HISTORY_H

typedef struct {
    int enabled;
    int initialized;
    void *data;
    int data_size;
} Clipboard_historyState;

int  clipboard_history_init(Clipboard_historyState *s);
int  clipboard_history_update(Clipboard_historyState *s);
void clipboard_history_destroy(Clipboard_historyState *s);

#endif
