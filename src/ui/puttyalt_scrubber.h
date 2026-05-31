#ifndef PUTTYALT_SCRUBBER_H
#define PUTTYALT_SCRUBBER_H

/*
 * puttyalt_scrubber - replay scrubber timeline.
 *
 * Drives the playback timeline UI for a recorded session: maps between the
 * playhead time and a horizontal pixel track, supports seek/scrub, variable
 * playback speed, and chapter markers placed on the timeline. Knows nothing
 * about how frames are stored; it only owns the timeline cursor.
 *
 * Functions use the unique scrub_ prefix.
 */

#include <stdint.h>

#define SCRUB_MAX_MARKERS   32
#define SCRUB_LABEL_LEN     32

typedef struct {
    uint32_t time_ms;                 /* position on the timeline */
    char     label[SCRUB_LABEL_LEN];
} ScrubMarker;

typedef struct {
    uint32_t    duration_ms;          /* total timeline length */
    uint32_t    playhead_ms;          /* current position */
    float       speed;                /* playback multiplier, >0 */
    int         playing;
    int         track_px;             /* pixel width of the scrub track */
    ScrubMarker markers[SCRUB_MAX_MARKERS];
    int         marker_count;
} ScrubTimeline;

void scrub_init(ScrubTimeline *s, uint32_t duration_ms, int track_px);

/* Advance the playhead by `elapsed_ms` of wall time scaled by speed.
 * Returns 1 if playback reached the end (auto-pauses), else 0. */
int  scrub_advance(ScrubTimeline *s, uint32_t elapsed_ms);

void scrub_play(ScrubTimeline *s);
void scrub_pause(ScrubTimeline *s);
void scrub_toggle(ScrubTimeline *s);
int  scrub_set_speed(ScrubTimeline *s, float speed);

void scrub_seek_ms(ScrubTimeline *s, uint32_t time_ms);

/* Map a pixel X on the track (0..track_px) to a timeline ms and seek there. */
void scrub_seek_px(ScrubTimeline *s, int x_px);

/* Current playhead as a pixel X on the track. */
int  scrub_playhead_px(const ScrubTimeline *s);

/* Progress fraction 0.0..1.0. */
float scrub_progress(const ScrubTimeline *s);

int  scrub_add_marker(ScrubTimeline *s, uint32_t time_ms, const char *label);

/* Nearest marker at/after the playhead (skip to next chapter). Returns its
 * index, or -1 if none ahead. */
int  scrub_next_marker(const ScrubTimeline *s);
int  scrub_prev_marker(const ScrubTimeline *s);

/* Format "mm:ss / mm:ss" of playhead vs duration. Returns bytes written. */
int  scrub_format_time(const ScrubTimeline *s, char *buf, int buf_sz);

#endif /* PUTTYALT_SCRUBBER_H */
