#include "puttyalt_sessshare.h"
#include <string.h>
#include <stdio.h>
#include <time.h>

int sessshare_init(SessionShare *ss)
{
    memset(ss, 0, sizeof(*ss));
    ss->port = SESSSHARE_PORT_DEFAULT;
    ss->max_viewers = SESSSHARE_MAX_VIEWERS;
    ss->encryption = 1;
    return 0;
}

void sessshare_destroy(SessionShare *ss)
{
    if (ss->state == SHARE_STATE_HOSTING)
        sessshare_host_stop(ss);
    else if (ss->state == SHARE_STATE_VIEWING)
        sessshare_view_disconnect(ss);
    memset(ss, 0, sizeof(*ss));
}

int sessshare_host_start(SessionShare *ss, int port, const char *password)
{
    if (ss->state != SHARE_STATE_IDLE) return -1;
    ss->port = port > 0 ? port : SESSSHARE_PORT_DEFAULT;
    if (password && password[0])
        snprintf(ss->password, sizeof(ss->password), "%s", password);
    ss->state = SHARE_STATE_HOSTING;
    ss->started_at = (long)time(NULL);
    ss->viewer_count = 0;
    ss->total_bytes = 0;
    return 0;
}

int sessshare_host_stop(SessionShare *ss)
{
    if (ss->state != SHARE_STATE_HOSTING) return -1;
    ss->state = SHARE_STATE_IDLE;
    ss->viewer_count = 0;
    return 0;
}

int sessshare_view_connect(SessionShare *ss, const char *host, int port,
                           const char *password)
{
    if (ss->state != SHARE_STATE_IDLE) return -1;
    (void)host; (void)port; (void)password;
    ss->state = SHARE_STATE_VIEWING;
    ss->started_at = (long)time(NULL);
    return 0;
}

int sessshare_view_disconnect(SessionShare *ss)
{
    if (ss->state != SHARE_STATE_VIEWING) return -1;
    ss->state = SHARE_STATE_IDLE;
    return 0;
}

int sessshare_kick_viewer(SessionShare *ss, int idx)
{
    if (idx < 0 || idx >= ss->viewer_count) return -1;
    for (int i = idx; i < ss->viewer_count - 1; i++)
        ss->viewers[i] = ss->viewers[i + 1];
    ss->viewer_count--;
    return 0;
}

int sessshare_set_mode(SessionShare *ss, ShareMode mode)
{
    ss->mode = mode;
    return 0;
}

int sessshare_broadcast_data(SessionShare *ss, const unsigned char *data, int len)
{
    if (ss->state != SHARE_STATE_HOSTING || !data || len <= 0) return -1;
    ss->total_bytes += (unsigned long)len;
    for (int i = 0; i < ss->viewer_count; i++)
        ss->viewers[i].bytes_sent += (unsigned long)len;
    return len;
}
