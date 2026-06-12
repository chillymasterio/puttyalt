/* puttyalt_sessstats.c - Session statistics aggregator (bytes/duration/cmds). */
#include <string.h>
#include <stdint.h>
typedef struct { uint64_t bytes_in, bytes_out; uint64_t commands; uint64_t duration_sec; uint64_t reconnects; uint64_t errors; } SessStats;
void sessstats_init(SessStats *s) { if(s) memset(s,0,sizeof(*s)); }
void sessstats_add_in(SessStats *s, uint64_t bytes) { if(s) s->bytes_in+=bytes; }
void sessstats_add_out(SessStats *s, uint64_t bytes) { if(s) s->bytes_out+=bytes; }
void sessstats_command(SessStats *s) { if(s) s->commands++; }
void sessstats_reconnect(SessStats *s) { if(s) s->reconnects++; }
void sessstats_error(SessStats *s) { if(s) s->errors++; }
void sessstats_set_duration(SessStats *s, uint64_t sec) { if(s) s->duration_sec=sec; }
uint64_t sessstats_total_bytes(const SessStats *s) { return s?(s->bytes_in+s->bytes_out):0; }
uint64_t sessstats_avg_throughput(const SessStats *s) {
    if(!s||s->duration_sec==0) return 0;
    return (s->bytes_in+s->bytes_out)/s->duration_sec;
}
void sessstats_merge(SessStats *dst, const SessStats *src) {
    if(!dst||!src) return;
    dst->bytes_in+=src->bytes_in; dst->bytes_out+=src->bytes_out; dst->commands+=src->commands;
    dst->duration_sec+=src->duration_sec; dst->reconnects+=src->reconnects; dst->errors+=src->errors;
}
