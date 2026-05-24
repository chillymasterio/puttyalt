#include "puttyalt_clipsync.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

void clipsync_init(ClipSync *cs)
{
    memset(cs, 0, sizeof(*cs));
    cs->enabled = 1;
    cs->auto_sync = 1;
}

int clipsync_add_session(ClipSync *cs, int session_id)
{
    if (cs->session_count >= CSYNC_MAX_SESSIONS) return -1;
    for (int i = 0; i < cs->session_count; i++)
        if (cs->session_ids[i] == session_id) return i;
    cs->session_ids[cs->session_count] = session_id;
    return cs->session_count++;
}

int clipsync_remove_session(ClipSync *cs, int session_id)
{
    for (int i = 0; i < cs->session_count; i++) {
        if (cs->session_ids[i] == session_id) {
            for (int j = i; j < cs->session_count - 1; j++)
                cs->session_ids[j] = cs->session_ids[j + 1];
            cs->session_count--;
            return 0;
        }
    }
    return -1;
}

int clipsync_push(ClipSync *cs, const char *text, int source_session)
{
    if (!cs->enabled || !text || !*text) return -1;

    /* Skip if same as current */
    if (strcmp(cs->current, text) == 0) return 0;

    snprintf(cs->current, CSYNC_MAX_TEXT, "%s", text);

    /* Add to history */
    CSyncEntry *e = &cs->history[cs->hist_pos];
    snprintf(e->text, CSYNC_MAX_TEXT, "%s", text);
    e->timestamp = (long)time(NULL);
    e->source_session = source_session;
    cs->hist_pos = (cs->hist_pos + 1) % CSYNC_MAX_HISTORY;
    if (cs->hist_count < CSYNC_MAX_HISTORY) cs->hist_count++;

    return cs->session_count; /* number of sessions to sync to */
}

const char *clipsync_get(const ClipSync *cs)
{
    return cs->current[0] ? cs->current : NULL;
}

int clipsync_get_history(const ClipSync *cs, int index, char *buf, int bufsz)
{
    if (index < 0 || index >= cs->hist_count) return -1;
    int real = (cs->hist_pos - cs->hist_count + index + CSYNC_MAX_HISTORY) % CSYNC_MAX_HISTORY;
    snprintf(buf, bufsz, "%s", cs->history[real].text);
    return 0;
}

void clipsync_clear_history(ClipSync *cs)
{
    cs->hist_count = 0;
    cs->hist_pos = 0;
}

void clipsync_enable(ClipSync *cs, int enable)
{
    cs->enabled = enable;
}
