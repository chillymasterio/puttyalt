#include <string.h>
#include <stdio.h>
#include "puttyalt_stats.h"

void stats_init(SessionStats *s) { memset(s, 0, sizeof(*s)); }

void stats_record_sent(SessionStats *s, unsigned long bytes)
{ s->bytes_sent += bytes; }

void stats_record_recv(SessionStats *s, unsigned long bytes)
{ s->bytes_received += bytes; }

void stats_record_keystroke(SessionStats *s) { s->keystrokes++; }

void stats_record_command(SessionStats *s, const char *cmd)
{
    s->commands_run++;
    if (cmd) strncpy(s->last_command, cmd, sizeof(s->last_command) - 1);
}

void stats_record_latency(SessionStats *s, double ms)
{
    if (ms > s->peak_latency_ms) s->peak_latency_ms = ms;
    /* Running average */
    double n = (double)s->commands_run;
    if (n > 0) s->avg_latency_ms = (s->avg_latency_ms * (n - 1) + ms) / n;
    else s->avg_latency_ms = ms;
}

void stats_format(const SessionStats *s, char *buf, int buflen)
{
    snprintf(buf, buflen,
        "Sent: %lu bytes | Recv: %lu bytes | Keys: %lu\n"
        "Commands: %lu | Reconnects: %d\n"
        "Avg Latency: %.1f ms | Peak: %.1f ms\n",
        s->bytes_sent, s->bytes_received, s->keystrokes,
        s->commands_run, s->reconnect_count,
        s->avg_latency_ms, s->peak_latency_ms);
}
