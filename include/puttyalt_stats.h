#ifndef PUTTYALT_STATS_H
#define PUTTYALT_STATS_H

typedef struct SessionStats {
    unsigned long bytes_sent;
    unsigned long bytes_received;
    unsigned long keystrokes;
    unsigned long commands_run;
    unsigned long connect_time;     /* epoch */
    unsigned long total_connected;  /* seconds */
    unsigned long total_idle;       /* seconds */
    int reconnect_count;
    double avg_latency_ms;
    double peak_latency_ms;
    char last_command[256];
} SessionStats;

void stats_init(SessionStats *s);
void stats_record_sent(SessionStats *s, unsigned long bytes);
void stats_record_recv(SessionStats *s, unsigned long bytes);
void stats_record_keystroke(SessionStats *s);
void stats_record_command(SessionStats *s, const char *cmd);
void stats_record_latency(SessionStats *s, double ms);
void stats_format(const SessionStats *s, char *buf, int buflen);

#endif
