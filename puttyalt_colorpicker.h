#ifndef PUTTYALT_COLORPICKER_H
#define PUTTYALT_COLORPICKER_H

typedef struct {
    int enabled;
    int initialized;
    void *data;
    int data_size;
} ColorpickerState;

int  colorpicker_init(ColorpickerState *s);
int  colorpicker_update(ColorpickerState *s);
void colorpicker_destroy(ColorpickerState *s);

#endif
