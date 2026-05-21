#ifndef PUTTYALT_MINIMAP_H
#define PUTTYALT_MINIMAP_H

typedef struct {
    int enabled;
    int initialized;
    void *data;
    int data_size;
} MinimapState;

int  minimap_init(MinimapState *s);
int  minimap_update(MinimapState *s);
void minimap_destroy(MinimapState *s);

#endif
