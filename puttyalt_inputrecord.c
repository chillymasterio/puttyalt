#include "puttyalt_inputrecord.h"
#include <string.h>
#include <stdio.h>
#include <time.h>

void irec_init(InputRecorder *ir) { memset(ir, 0, sizeof(*ir)); ir->speed_multiplier = 100; }

void irec_start_recording(InputRecorder *ir, const char *name)
{
    ir->count = 0;
    ir->recording = 1;
    ir->playing = 0;
    snprintf(ir->name, sizeof(ir->name), "%s", name ? name : "Macro");
    ir->start_time = (unsigned long)time(NULL);
}

void irec_stop_recording(InputRecorder *ir) { ir->recording = 0; }

int irec_add_key(InputRecorder *ir, const char *key, int delay_ms)
{
    if (!ir->recording || ir->count >= IREC_MAX_EVENTS) return -1;
    InputEvent *e = &ir->events[ir->count++];
    e->type = 0;
    snprintf(e->data, sizeof(e->data), "%s", key);
    e->delay_ms = delay_ms;
    return 0;
}

int irec_start_playback(InputRecorder *ir, int repeat)
{
    if (ir->count == 0) return -1;
    ir->playing = 1;
    ir->play_index = 0;
    ir->repeat_count = repeat > 0 ? repeat : 1;
    return 0;
}

int irec_next_event(InputRecorder *ir, InputEvent *out)
{
    if (!ir->playing || ir->play_index >= ir->count) {
        if (ir->repeat_count > 1) {
            ir->repeat_count--;
            ir->play_index = 0;
        } else {
            ir->playing = 0;
            return -1;
        }
    }
    *out = ir->events[ir->play_index++];
    out->delay_ms = out->delay_ms * 100 / ir->speed_multiplier;
    return 0;
}

int irec_is_recording(InputRecorder *ir) { return ir->recording; }
int irec_is_playing(InputRecorder *ir) { return ir->playing; }
void irec_stop_playback(InputRecorder *ir) { ir->playing = 0; }

int irec_save(InputRecorder *ir, const char *path)
{
    FILE *f = fopen(path, "w");
    if (!f) return -1;
    fprintf(f, "# PuttyAlt Macro: %s\n# Events: %d\n", ir->name, ir->count);
    for (int i = 0; i < ir->count; i++) {
        InputEvent *e = &ir->events[i];
        fprintf(f, "%d\t%d\t%s\n", e->type, e->delay_ms, e->data);
    }
    fclose(f);
    return 0;
}

int irec_load(InputRecorder *ir, const char *path)
{
    FILE *f = fopen(path, "r");
    if (!f) return -1;
    ir->count = 0;
    char line[256];
    while (fgets(line, sizeof(line), f) && ir->count < IREC_MAX_EVENTS) {
        if (line[0] == '#') continue;
        InputEvent *e = &ir->events[ir->count];
        char data[64];
        if (sscanf(line, "%d\t%d\t%63[^\n]", &e->type, &e->delay_ms, data) >= 3) {
            snprintf(e->data, sizeof(e->data), "%s", data);
            ir->count++;
        }
    }
    fclose(f);
    return 0;
}
