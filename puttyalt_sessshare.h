#ifndef PUTTYALT_SESSSHARE_H
#define PUTTYALT_SESSSHARE_H

#define SESSSHARE_MAX_VIEWERS  8
#define SESSSHARE_PORT_DEFAULT 9922

typedef enum {
    SHARE_MODE_READONLY,
    SHARE_MODE_INTERACTIVE,
    SHARE_MODE_PRESENTER
} ShareMode;

typedef enum {
    SHARE_STATE_IDLE,
    SHARE_STATE_HOSTING,
    SHARE_STATE_VIEWING,
    SHARE_STATE_ERROR
} ShareState;

typedef struct {
    char name[64];
    char ip[46];
    int port;
    ShareMode mode;
    long connected_at;
    unsigned long bytes_sent;
} ShareViewer;

typedef struct {
    ShareState state;
    ShareMode mode;
    int port;
    char password[64];
    ShareViewer viewers[SESSSHARE_MAX_VIEWERS];
    int viewer_count;
    int max_viewers;
    long started_at;
    int encryption;
    unsigned long total_bytes;
} SessionShare;

int  sessshare_init(SessionShare *ss);
void sessshare_destroy(SessionShare *ss);
int  sessshare_host_start(SessionShare *ss, int port, const char *password);
int  sessshare_host_stop(SessionShare *ss);
int  sessshare_view_connect(SessionShare *ss, const char *host, int port,
                            const char *password);
int  sessshare_view_disconnect(SessionShare *ss);
int  sessshare_kick_viewer(SessionShare *ss, int idx);
int  sessshare_set_mode(SessionShare *ss, ShareMode mode);
int  sessshare_broadcast_data(SessionShare *ss, const unsigned char *data, int len);

#endif
