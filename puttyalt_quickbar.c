#include "puttyalt_quickbar.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void quickbar_init(QuickBar *qb)
{
    memset(qb, 0, sizeof(*qb));
    snprintf(qb->placeholder, sizeof(qb->placeholder), "user@hostname:port");
}

void quickbar_destroy(QuickBar *qb)
{
    for (int i = 0; i < qb->history_count; i++) free(qb->history[i]);
    memset(qb, 0, sizeof(*qb));
}

void quickbar_show(QuickBar *qb) { qb->visible = 1; qb->focused = 1; }
void quickbar_hide(QuickBar *qb) { qb->visible = 0; qb->focused = 0; }
void quickbar_toggle(QuickBar *qb) { qb->visible ? quickbar_hide(qb) : quickbar_show(qb); }

int quickbar_parse(QuickBar *qb, char *host, int *port, char *user)
{
    char buf[512];
    snprintf(buf, sizeof(buf), "%s", qb->input);
    *port = 22; host[0] = user[0] = '\0';

    char *at = strchr(buf, '@');
    char *start = buf;
    if (at) { *at = '\0'; snprintf(user, 128, "%s", buf); start = at + 1; }
    char *colon = strchr(start, ':');
    if (colon) { *colon = '\0'; *port = atoi(colon + 1); }
    snprintf(host, 256, "%s", start);
    return host[0] ? 0 : -1;
}

void quickbar_add_history(QuickBar *qb, const char *entry)
{
    if (qb->history_count >= 32) { free(qb->history[0]); memmove(&qb->history[0], &qb->history[1], 31 * sizeof(char *)); qb->history_count = 31; }
    qb->history[qb->history_count++] = strdup(entry);
    qb->history_pos = qb->history_count;
}

void quickbar_history_prev(QuickBar *qb)
{
    if (qb->history_pos > 0) { qb->history_pos--; snprintf(qb->input, sizeof(qb->input), "%s", qb->history[qb->history_pos]); }
}

void quickbar_history_next(QuickBar *qb)
{
    if (qb->history_pos < qb->history_count - 1) { qb->history_pos++; snprintf(qb->input, sizeof(qb->input), "%s", qb->history[qb->history_pos]); }
    else { qb->history_pos = qb->history_count; qb->input[0] = '\0'; }
}
