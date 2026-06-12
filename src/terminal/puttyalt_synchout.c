/* puttyalt_synchout.c - Synchronized output (DEC 2026) frame batching guard. */
#include <stdint.h>
typedef struct { int active; uint64_t begin_ms; int timeout_ms; int frames_buffered; int frames_flushed; } SynchOut;
void synchout_init(SynchOut *s, int timeout_ms) {
    if(!s) return;
    s->active=0; s->begin_ms=0; s->timeout_ms=timeout_ms>0?timeout_ms:150; s->frames_buffered=0; s->frames_flushed=0;
}
int synchout_begin(SynchOut *s, uint64_t now_ms) {
    if(!s) return -1;
    if (s->active) return 1; /* already in sync block */
    s->active=1; s->begin_ms=now_ms; return 0;
}
int synchout_end(SynchOut *s) {
    if(!s||!s->active) return -1;
    s->active=0; s->frames_flushed+=s->frames_buffered; s->frames_buffered=0; return 0;
}
int synchout_buffer(SynchOut *s) { if(!s||!s->active) return -1; s->frames_buffered++; return 0; }
int synchout_should_force_flush(const SynchOut *s, uint64_t now_ms) {
    if(!s||!s->active) return 0;
    return (now_ms-s->begin_ms) >= (uint64_t)s->timeout_ms ? 1 : 0;
}
int synchout_is_active(const SynchOut *s) { return s?s->active:-1; }
