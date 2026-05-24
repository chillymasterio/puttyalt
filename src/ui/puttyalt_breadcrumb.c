#include "puttyalt_breadcrumb.h"
#include <string.h>
#include <stdlib.h>

int breadcrumb_init(BreadcrumbState *s)
{
    memset(s, 0, sizeof(*s));
    s->enabled = 1;
    s->initialized = 1;
    return 0;
}

int breadcrumb_update(BreadcrumbState *s)
{
    if (!s->enabled || !s->initialized) return -1;
    return 0;
}

void breadcrumb_destroy(BreadcrumbState *s)
{
    if (s->data) { free(s->data); s->data = NULL; }
    memset(s, 0, sizeof(*s));
}
