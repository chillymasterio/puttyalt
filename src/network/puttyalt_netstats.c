#include <string.h>
#include <stdio.h>

typedef struct {
    unsigned long long bytes_sent;
    unsigned long long bytes_recv;
    unsigned long packets_sent;
    unsigned long packets_recv;
    unsigned long errors;
    double session_seconds;
} NetStats;

static NetStats g_stats;

void netstats_reset(void) { memset(&g_stats, 0, sizeof(g_stats)); }
void netstats_add_sent(int bytes) { g_stats.bytes_sent += bytes; g_stats.packets_sent++; }
void netstats_add_recv(int bytes) { g_stats.bytes_recv += bytes; g_stats.packets_recv++; }
void netstats_add_error(void) { g_stats.errors++; }
void netstats_set_duration(double secs) { g_stats.session_seconds = secs; }

int netstats_format(char *buf, int buflen)
{
    double sent_kb = g_stats.bytes_sent / 1024.0;
    double recv_kb = g_stats.bytes_recv / 1024.0;
    double rate = g_stats.session_seconds > 0 ?
        (g_stats.bytes_sent + g_stats.bytes_recv) / g_stats.session_seconds / 1024.0 : 0;
    return snprintf(buf, buflen,
        "Sent: %.1f KB (%lu pkts) | Recv: %.1f KB (%lu pkts) | Rate: %.1f KB/s | Errors: %lu",
        sent_kb, g_stats.packets_sent, recv_kb, g_stats.packets_recv, rate, g_stats.errors);
}

NetStats *netstats_get(void) { return &g_stats; }
