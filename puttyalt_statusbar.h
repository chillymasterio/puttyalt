/*
 * puttyalt_statusbar.h: Status bar with connection info.
 *
 * Shows at the bottom of the terminal window: connection state,
 * latency sparkline, transfer progress, and session name.
 */

#ifndef PUTTYALT_STATUSBAR_H
#define PUTTYALT_STATUSBAR_H

#define STATUSBAR_MAX_SECTIONS 8
#define STATUSBAR_SECTION_LEN  128

typedef enum {
    SBAR_SECTION_SESSION,     /* session name */
    SBAR_SECTION_STATE,       /* connected/disconnected */
    SBAR_SECTION_LATENCY,     /* sparkline */
    SBAR_SECTION_TRANSFER,    /* upload/download progress */
    SBAR_SECTION_ENCODING,    /* character encoding */
    SBAR_SECTION_SIZE,        /* terminal cols x rows */
    SBAR_SECTION_TIME,        /* uptime */
    SBAR_SECTION_ENV          /* environment indicator */
} StatusBarSection;

typedef struct StatusBar {
    char sections[STATUSBAR_MAX_SECTIONS][STATUSBAR_SECTION_LEN];
    int  visible;
    int  height_px;
} StatusBar;

void statusbar_init(StatusBar *sb);
void statusbar_set(StatusBar *sb, StatusBarSection section, const char *text);
void statusbar_clear(StatusBar *sb);
void statusbar_toggle(StatusBar *sb);
const char *statusbar_get(const StatusBar *sb, StatusBarSection section);

#endif /* PUTTYALT_STATUSBAR_H */
