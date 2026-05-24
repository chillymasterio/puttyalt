#include "puttyalt_conndash.h"
#include <string.h>
#include <stdio.h>
#include <time.h>

void conndash_init(ConnDashboard *cd)
{
    memset(cd, 0, sizeof(*cd));
    cd->refresh_interval_sec = 5;
}

int conndash_add(ConnDashboard *cd, int id, const char *name, const char *host, int port)
{
    if (cd->count >= DASH_MAX_SESSIONS) return -1;
    DashSession *s = &cd->sessions[cd->count++];
    memset(s, 0, sizeof(*s));
    s->id = id;
    snprintf(s->name, sizeof(s->name), "%s", name);
    snprintf(s->host, sizeof(s->host), "%s", host);
    s->port = port;
    s->status = SESS_DISCONNECTED;
    return 0;
}

int conndash_update_status(ConnDashboard *cd, int id, SessionStatus status)
{
    DashSession *s = conndash_find(cd, id);
    if (!s) return -1;
    s->status = status;
    if (status == SESS_CONNECTED) s->connected_since = (unsigned long)time(NULL);
    return 0;
}

int conndash_update_stats(ConnDashboard *cd, int id, int latency, unsigned long in, unsigned long out)
{
    DashSession *s = conndash_find(cd, id);
    if (!s) return -1;
    s->latency_ms = latency;
    s->bytes_in = in;
    s->bytes_out = out;
    return 0;
}

DashSession *conndash_find(ConnDashboard *cd, int id)
{
    for (int i = 0; i < cd->count; i++) if (cd->sessions[i].id == id) return &cd->sessions[i];
    return NULL;
}

void conndash_remove(ConnDashboard *cd, int id)
{
    for (int i = 0; i < cd->count; i++) {
        if (cd->sessions[i].id == id) {
            memmove(&cd->sessions[i], &cd->sessions[i+1], (cd->count-i-1)*sizeof(DashSession));
            cd->count--;
            return;
        }
    }
}

int conndash_count_by_status(ConnDashboard *cd, SessionStatus status)
{
    int n = 0;
    for (int i = 0; i < cd->count; i++) if (cd->sessions[i].status == status) n++;
    return n;
}

void conndash_sort(ConnDashboard *cd)
{
    /* insertion sort by selected criteria */
    for (int i = 1; i < cd->count; i++) {
        DashSession tmp = cd->sessions[i];
        int j = i - 1;
        int swap = 0;
        while (j >= 0) {
            switch (cd->sort_by) {
                case 0: swap = strcmp(cd->sessions[j].name, tmp.name) > 0; break;
                case 1: swap = cd->sessions[j].status > tmp.status; break;
                case 2: swap = cd->sessions[j].latency_ms > tmp.latency_ms; break;
                case 3: swap = (cd->sessions[j].bytes_in + cd->sessions[j].bytes_out) < (tmp.bytes_in + tmp.bytes_out); break;
            }
            if (!swap) break;
            cd->sessions[j+1] = cd->sessions[j];
            j--;
        }
        cd->sessions[j+1] = tmp;
    }
}

void conndash_toggle(ConnDashboard *cd) { cd->visible = !cd->visible; }
