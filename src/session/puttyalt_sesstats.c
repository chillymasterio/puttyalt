/* puttyalt_sesstats.c - Aggregate session usage statistics.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
typedef struct { long total_connects; long total_seconds; long bytes_in; long bytes_out; int sessions; } SessStats;
void sst_init(SessStats *s) { if (s) { s->total_connects=0; s->total_seconds=0; s->bytes_in=0; s->bytes_out=0; s->sessions=0; } }
void sst_record(SessStats *s, long seconds, long in, long out) {
    if (!s) return;
    s->total_connects++; s->total_seconds += seconds;
    s->bytes_in += in; s->bytes_out += out;
}
long sst_avg_duration(const SessStats *s) {
    return (s && s->total_connects) ? s->total_seconds / s->total_connects : 0;
}
long sst_total_bytes(const SessStats *s) { return s ? s->bytes_in + s->bytes_out : 0; }
