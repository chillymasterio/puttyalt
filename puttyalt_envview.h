#ifndef PUTTYALT_ENVVIEW_H
#define PUTTYALT_ENVVIEW_H

#define ENV_MAX 256

typedef struct {
    char name[128];
    char value[512];
    int  modified;
} EnvEntry;

typedef struct {
    EnvEntry entries[ENV_MAX];
    int count;
    char filter[128];
} EnvViewer;

void envview_init(EnvViewer *ev);
int  envview_add(EnvViewer *ev, const char *name, const char *value);
int  envview_set(EnvViewer *ev, const char *name, const char *value);
const char *envview_get(EnvViewer *ev, const char *name);
int  envview_filter(EnvViewer *ev, const char *pattern, int *results, int max);
void envview_clear(EnvViewer *ev);

#endif
