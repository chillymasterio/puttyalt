#include "puttyalt_colorpicker.h"
#include <string.h>
#include <stdlib.h>

int colorpicker_init(ColorpickerState *s)
{
    memset(s, 0, sizeof(*s));
    s->enabled = 1;
    s->initialized = 1;
    return 0;
}

int colorpicker_update(ColorpickerState *s)
{
    if (!s->enabled || !s->initialized) return -1;
    return 0;
}

void colorpicker_destroy(ColorpickerState *s)
{
    if (s->data) { free(s->data); s->data = NULL; }
    memset(s, 0, sizeof(*s));
}
