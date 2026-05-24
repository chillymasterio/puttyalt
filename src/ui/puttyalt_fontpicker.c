#include "puttyalt_fontpicker.h"
#include <string.h>
#include <stdlib.h>

int fontpicker_init(FontpickerState *s)
{
    memset(s, 0, sizeof(*s));
    s->enabled = 1;
    s->initialized = 1;
    return 0;
}

int fontpicker_update(FontpickerState *s)
{
    if (!s->enabled || !s->initialized) return -1;
    return 0;
}

void fontpicker_destroy(FontpickerState *s)
{
    if (s->data) { free(s->data); s->data = NULL; }
    memset(s, 0, sizeof(*s));
}
