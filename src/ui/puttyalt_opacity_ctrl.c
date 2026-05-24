#include "puttyalt_opacity_ctrl.h"
#include <string.h>
#include <stdlib.h>

int opacity_ctrl_init(Opacity_ctrlState *s)
{
    memset(s, 0, sizeof(*s));
    s->enabled = 1;
    s->initialized = 1;
    return 0;
}

int opacity_ctrl_update(Opacity_ctrlState *s)
{
    if (!s->enabled || !s->initialized) return -1;
    return 0;
}

void opacity_ctrl_destroy(Opacity_ctrlState *s)
{
    if (s->data) { free(s->data); s->data = NULL; }
    memset(s, 0, sizeof(*s));
}
