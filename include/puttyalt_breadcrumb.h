#ifndef PUTTYALT_BREADCRUMB_H
#define PUTTYALT_BREADCRUMB_H

typedef struct {
    int enabled;
    int initialized;
    void *data;
    int data_size;
} BreadcrumbState;

int  breadcrumb_init(BreadcrumbState *s);
int  breadcrumb_update(BreadcrumbState *s);
void breadcrumb_destroy(BreadcrumbState *s);

#endif
