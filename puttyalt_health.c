/*
 * puttyalt_health.c: Connection health monitoring and statistics.
 *
 * Tracks connection uptime, bytes transferred, latency estimates,
 * and reconnect history. Displayed in the status bar and available
 * through a Connection Info dialog.
 */

#include <stdio.h>
#include <string.h>
#include <time.h>

typedef struct ConnectionHealth {
    /* Connection state */
    int connected;
    time_t connect_time;       /* when current connection was established */
    time_t disconnect_time;    /* when last disconnected (0 if connected) */

    /* Transfer statistics */
    unsigned long long bytes_sent;
    unsigned long long bytes_received;

    /* Reconnect tracking */
    int reconnect_count;
    int failed_reconnect_count;

    /* Session totals (across reconnects) */
    time_t session_start;
    unsigned long long session_bytes_sent;
    unsigned long long session_bytes_received;
} ConnectionHealth;

void health_init(ConnectionHealth *h)
{
    memset(h, 0, sizeof(*h));
    h->session_start = time(NULL);
}

void health_on_connect(ConnectionHealth *h)
{
    h->connected = 1;
    h->connect_time = time(NULL);
    h->disconnect_time = 0;
}

void health_on_disconnect(ConnectionHealth *h)
{
    h->connected = 0;
    h->disconnect_time = time(NULL);
}

void health_on_reconnect(ConnectionHealth *h, int success)
{
    if (success) {
        h->reconnect_count++;
        health_on_connect(h);
    } else {
        h->failed_reconnect_count++;
    }
}

void health_add_bytes(ConnectionHealth *h,
                      unsigned long long sent,
                      unsigned long long received)
{
    h->bytes_sent += sent;
    h->bytes_received += received;
    h->session_bytes_sent += sent;
    h->session_bytes_received += received;
}

/*
 * Format uptime as human-readable string.
 * Returns pointer to static buffer.
 */
const char *health_uptime_str(const ConnectionHealth *h)
{
    static char buf[64];
    if (!h->connected) {
        snprintf(buf, sizeof(buf), "disconnected");
        return buf;
    }

    time_t now = time(NULL);
    long secs = (long)(now - h->connect_time);

    if (secs < 60)
        snprintf(buf, sizeof(buf), "%lds", secs);
    else if (secs < 3600)
        snprintf(buf, sizeof(buf), "%ldm %lds", secs / 60, secs % 60);
    else if (secs < 86400)
        snprintf(buf, sizeof(buf), "%ldh %ldm", secs / 3600, (secs % 3600) / 60);
    else
        snprintf(buf, sizeof(buf), "%ldd %ldh %ldm",
                 secs / 86400, (secs % 86400) / 3600, (secs % 3600) / 60);

    return buf;
}

/*
 * Format bytes as human-readable (B, KB, MB, GB).
 */
const char *health_bytes_str(unsigned long long bytes)
{
    static char buf[32];
    if (bytes < 1024)
        snprintf(buf, sizeof(buf), "%llu B", bytes);
    else if (bytes < 1024 * 1024)
        snprintf(buf, sizeof(buf), "%.1f KB", bytes / 1024.0);
    else if (bytes < 1024ULL * 1024 * 1024)
        snprintf(buf, sizeof(buf), "%.1f MB", bytes / (1024.0 * 1024));
    else
        snprintf(buf, sizeof(buf), "%.2f GB", bytes / (1024.0 * 1024 * 1024));
    return buf;
}

/*
 * Generate a one-line status string for the status bar.
 */
void health_status_line(const ConnectionHealth *h, char *buf, size_t len)
{
    if (!h->connected) {
        snprintf(buf, len, "Disconnected");
        return;
    }

    snprintf(buf, len, "Up %s | Sent %s | Recv %s",
             health_uptime_str(h),
             health_bytes_str(h->bytes_sent),
             health_bytes_str(h->bytes_received));

    if (h->reconnect_count > 0) {
        size_t used = strlen(buf);
        snprintf(buf + used, len - used, " | Reconnects: %d",
                 h->reconnect_count);
    }
}
