#include "puttyalt_scrubber.h"
#include <string.h>
#include <stdio.h>

void scrub_init(ScrubTimeline *s, uint32_t duration_ms, int track_px)
{
    memset(s, 0, sizeof(*s));
    s->duration_ms = duration_ms;
    s->track_px = track_px > 0 ? track_px : 1;
    s->speed = 1.0f;
}

int scrub_advance(ScrubTimeline *s, uint32_t elapsed_ms)
{
    if (!s->playing) return 0;
    uint32_t step = (uint32_t)((float)elapsed_ms * s->speed + 0.5f);
    if (step == 0 && elapsed_ms > 0 && s->speed > 0.0f) step = 1;

    if (s->playhead_ms + step >= s->duration_ms) {
        s->playhead_ms = s->duration_ms;
        s->playing = 0;
        return 1;
    }
    s->playhead_ms += step;
    return 0;
}

void scrub_play(ScrubTimeline *s)
{
    if (s->playhead_ms >= s->duration_ms)
        s->playhead_ms = 0;           /* restart from the top */
    s->playing = 1;
}

void scrub_pause(ScrubTimeline *s)
{
    s->playing = 0;
}

void scrub_toggle(ScrubTimeline *s)
{
    if (s->playing) scrub_pause(s);
    else            scrub_play(s);
}

int scrub_set_speed(ScrubTimeline *s, float speed)
{
    if (speed <= 0.0f || speed > 16.0f) return -1;
    s->speed = speed;
    return 0;
}

void scrub_seek_ms(ScrubTimeline *s, uint32_t time_ms)
{
    if (time_ms > s->duration_ms) time_ms = s->duration_ms;
    s->playhead_ms = time_ms;
}

void scrub_seek_px(ScrubTimeline *s, int x_px)
{
    if (x_px < 0) x_px = 0;
    if (x_px > s->track_px) x_px = s->track_px;
    /* 64-bit intermediate to avoid overflow for long recordings. */
    uint64_t t = (uint64_t)s->duration_ms * (uint64_t)x_px /
                 (uint64_t)s->track_px;
    s->playhead_ms = (uint32_t)t;
}

int scrub_playhead_px(const ScrubTimeline *s)
{
    if (s->duration_ms == 0) return 0;
    uint64_t px = (uint64_t)s->playhead_ms * (uint64_t)s->track_px /
                  (uint64_t)s->duration_ms;
    return (int)px;
}

float scrub_progress(const ScrubTimeline *s)
{
    if (s->duration_ms == 0) return 0.0f;
    return (float)s->playhead_ms / (float)s->duration_ms;
}

int scrub_add_marker(ScrubTimeline *s, uint32_t time_ms, const char *label)
{
    if (s->marker_count >= SCRUB_MAX_MARKERS) return -1;
    if (time_ms > s->duration_ms) return -1;

    /* keep markers sorted by time for next/prev navigation */
    int pos = s->marker_count;
    while (pos > 0 && s->markers[pos - 1].time_ms > time_ms) {
        s->markers[pos] = s->markers[pos - 1];
        pos--;
    }
    ScrubMarker *m = &s->markers[pos];
    memset(m, 0, sizeof(*m));
    m->time_ms = time_ms;
    if (label)
        snprintf(m->label, sizeof(m->label), "%s", label);
    s->marker_count++;
    return pos;
}

int scrub_next_marker(const ScrubTimeline *s)
{
    for (int i = 0; i < s->marker_count; i++)
        if (s->markers[i].time_ms > s->playhead_ms)
            return i;
    return -1;
}

int scrub_prev_marker(const ScrubTimeline *s)
{
    for (int i = s->marker_count - 1; i >= 0; i--)
        if (s->markers[i].time_ms < s->playhead_ms)
            return i;
    return -1;
}

int scrub_format_time(const ScrubTimeline *s, char *buf, int buf_sz)
{
    unsigned cur = s->playhead_ms / 1000;
    unsigned tot = s->duration_ms / 1000;
    return snprintf(buf, (size_t)buf_sz, "%02u:%02u / %02u:%02u",
                    cur / 60, cur % 60, tot / 60, tot % 60);
}
