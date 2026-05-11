/*
 * puttyalt_macro.c: Keyboard macro recording and playback.
 */

#include <string.h>
#include "puttyalt_macro.h"

void macro_init(MacroRecorder *rec)
{
    memset(rec, 0, sizeof(*rec));
    rec->recording = -1;
    rec->playing = -1;
    rec->play_pos = 0;
}

int macro_start_recording(MacroRecorder *rec, int slot, const char *name)
{
    if (rec->recording >= 0)
        return -1; /* already recording */
    if (slot < 0 || slot >= MACRO_MAX_SLOTS)
        return -1;

    Macro *m = &rec->slots[slot];
    memset(m, 0, sizeof(*m));
    if (name)
        strncpy(m->name, name, MACRO_NAME_LEN - 1);
    m->repeat_count = 0;

    rec->recording = slot;
    rec->last_event_time = 0;

    if (slot >= rec->num_macros)
        rec->num_macros = slot + 1;

    return 0;
}

int macro_stop_recording(MacroRecorder *rec)
{
    if (rec->recording < 0)
        return -1;
    rec->recording = -1;
    return 0;
}

int macro_record_event(MacroRecorder *rec, const unsigned char *data,
                       int len, unsigned long time_ms)
{
    if (rec->recording < 0)
        return -1;

    Macro *m = &rec->slots[rec->recording];
    if (m->num_events >= MACRO_MAX_EVENTS)
        return -1;
    if (len <= 0 || len > (int)sizeof(m->events[0].data))
        return -1;

    MacroEvent *ev = &m->events[m->num_events];
    memcpy(ev->data, data, len);
    ev->len = len;

    if (rec->last_event_time > 0 && time_ms > rec->last_event_time)
        ev->delay_ms = (unsigned int)(time_ms - rec->last_event_time);
    else
        ev->delay_ms = 0;

    rec->last_event_time = time_ms;
    m->num_events++;
    return 0;
}

int macro_play(MacroRecorder *rec, int slot)
{
    if (rec->playing >= 0)
        return -1; /* already playing */
    if (slot < 0 || slot >= MACRO_MAX_SLOTS || slot >= rec->num_macros)
        return -1;
    if (rec->slots[slot].num_events == 0)
        return -1;

    rec->playing = slot;
    rec->play_pos = 0;
    return 0;
}

int macro_stop_playback(MacroRecorder *rec)
{
    if (rec->playing < 0)
        return -1;
    rec->playing = -1;
    rec->play_pos = 0;
    return 0;
}

const MacroEvent *macro_next_event(MacroRecorder *rec)
{
    if (rec->playing < 0)
        return NULL;

    Macro *m = &rec->slots[rec->playing];
    if (rec->play_pos >= m->num_events) {
        if (m->repeat_count == -1) {
            rec->play_pos = 0; /* loop */
        } else if (m->repeat_count > 0) {
            m->repeat_count--;
            rec->play_pos = 0;
        } else {
            macro_stop_playback(rec);
            return NULL;
        }
    }

    return &m->events[rec->play_pos++];
}

int macro_delete(MacroRecorder *rec, int slot)
{
    if (slot < 0 || slot >= MACRO_MAX_SLOTS || slot >= rec->num_macros)
        return -1;
    memset(&rec->slots[slot], 0, sizeof(Macro));
    return 0;
}

int macro_rename(MacroRecorder *rec, int slot, const char *new_name)
{
    if (slot < 0 || slot >= MACRO_MAX_SLOTS || slot >= rec->num_macros)
        return -1;
    if (!new_name)
        return -1;
    strncpy(rec->slots[slot].name, new_name, MACRO_NAME_LEN - 1);
    rec->slots[slot].name[MACRO_NAME_LEN - 1] = '\0';
    return 0;
}
