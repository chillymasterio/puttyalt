#ifndef PUTTYALT_FONTPICKER_H
#define PUTTYALT_FONTPICKER_H

typedef struct {
    int enabled;
    int initialized;
    void *data;
    int data_size;
} FontpickerState;

int  fontpicker_init(FontpickerState *s);
int  fontpicker_update(FontpickerState *s);
void fontpicker_destroy(FontpickerState *s);

#endif
