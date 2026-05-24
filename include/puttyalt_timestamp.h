#ifndef PUTTYALT_TIMESTAMP_H
#define PUTTYALT_TIMESTAMP_H

typedef struct {
    int  enabled;
    int  format; /* 0=HH:MM:SS, 1=HH:MM:SS.ms, 2=ISO8601, 3=relative */
    int  position; /* 0=left margin, 1=right margin */
    int  color;
    int  on_newline_only;
    int  show_elapsed; /* show time since last line */
    unsigned long last_line_time;
} TimestampConfig;

void timestamp_init(TimestampConfig *tc);
int  timestamp_format(TimestampConfig *tc, char *buf, int buflen);
int  timestamp_elapsed(TimestampConfig *tc, char *buf, int buflen);
void timestamp_mark(TimestampConfig *tc);
void timestamp_toggle(TimestampConfig *tc);

#endif
