/*
 * puttyalt_health.h: Connection health monitor.
 *
 * Tracks uptime, data transfer volume, and reconnection statistics
 * for each SSH session.  Displayed in the status bar.
 */

#ifndef PUTTYALT_HEALTH_H
#define PUTTYALT_HEALTH_H

typedef struct ConnectionHealth {
    long   connect_time;     /* timestamp of last connect */
    long   disconnect_time;  /* timestamp of last disconnect */
    long   total_bytes_in;
    long   total_bytes_out;
    int    reconnect_count;
    int    connected;
} ConnectionHealth;

void health_init(ConnectionHealth *h);
void health_on_connect(ConnectionHealth *h, long now);
void health_on_disconnect(ConnectionHealth *h, long now);
void health_on_reconnect(ConnectionHealth *h, long now);
void health_add_bytes(ConnectionHealth *h, long in_bytes, long out_bytes);
long health_uptime_seconds(const ConnectionHealth *h, long now);
void health_uptime_str(const ConnectionHealth *h, long now, char *buf, int bufsize);
void health_bytes_str(long bytes, char *buf, int bufsize);
void health_status_line(const ConnectionHealth *h, long now, char *buf, int bufsize);

#endif /* PUTTYALT_HEALTH_H */
