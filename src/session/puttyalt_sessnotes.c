#include "puttyalt_sessnotes.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

void snotes_init(SessionNotes *sn) { memset(sn, 0, sizeof(*sn)); }

int snotes_add(SessionNotes *sn, const char *title, const char *body, const char *session)
{
    if (sn->count >= SN_MAX_NOTES) return -1;
    SessionNote *n = &sn->notes[sn->count];
    memset(n, 0, sizeof(*n));
    snprintf(n->title, SN_MAX_TITLE, "%s", title);
    if (body) snprintf(n->body, SN_MAX_BODY, "%s", body);
    if (session) snprintf(n->session_name, sizeof(n->session_name), "%s", session);
    n->created = n->modified = (long)time(NULL);
    return sn->count++;
}

int snotes_update(SessionNotes *sn, int index, const char *title, const char *body)
{
    if (index < 0 || index >= sn->count) return -1;
    if (title) snprintf(sn->notes[index].title, SN_MAX_TITLE, "%s", title);
    if (body) snprintf(sn->notes[index].body, SN_MAX_BODY, "%s", body);
    sn->notes[index].modified = (long)time(NULL);
    return 0;
}

int snotes_remove(SessionNotes *sn, int index)
{
    if (index < 0 || index >= sn->count) return -1;
    for (int i = index; i < sn->count - 1; i++)
        sn->notes[i] = sn->notes[i + 1];
    sn->count--;
    return 0;
}

int snotes_add_tag(SessionNotes *sn, int index, const char *tag)
{
    if (index < 0 || index >= sn->count) return -1;
    SessionNote *n = &sn->notes[index];
    if (n->tag_count >= SN_MAX_TAGS) return -1;
    snprintf(n->tags[n->tag_count], SN_MAX_TAG, "%s", tag);
    return n->tag_count++;
}

int snotes_search(const SessionNotes *sn, const char *query, int *results, int max)
{
    int n = 0;
    for (int i = 0; i < sn->count && n < max; i++) {
        if (strstr(sn->notes[i].title, query) || strstr(sn->notes[i].body, query))
            results[n++] = i;
    }
    return n;
}

int snotes_by_session(const SessionNotes *sn, const char *session, int *results, int max)
{
    int n = 0;
    for (int i = 0; i < sn->count && n < max; i++) {
        if (strcmp(sn->notes[i].session_name, session) == 0)
            results[n++] = i;
    }
    return n;
}

void snotes_pin(SessionNotes *sn, int index, int pin)
{
    if (index >= 0 && index < sn->count) sn->notes[index].pinned = pin;
}

int snotes_load(SessionNotes *sn, const char *path)
{
    FILE *f = fopen(path, "r");
    char line[4096];
    if (!f) return -1;
    snotes_init(sn);
    SessionNote *cur = NULL;
    while (fgets(line, sizeof(line), f)) {
        size_t len = strlen(line);
        while (len > 0 && (line[len-1] == '\n' || line[len-1] == '\r')) line[--len] = '\0';
        if (strcmp(line, "[note]") == 0) {
            if (sn->count >= SN_MAX_NOTES) break;
            cur = &sn->notes[sn->count++];
            memset(cur, 0, sizeof(*cur));
            continue;
        }
        if (!cur) continue;
        if (strncmp(line, "title=", 6) == 0) snprintf(cur->title, SN_MAX_TITLE, "%s", line+6);
        else if (strncmp(line, "body=", 5) == 0) snprintf(cur->body, SN_MAX_BODY, "%s", line+5);
        else if (strncmp(line, "session=", 8) == 0) snprintf(cur->session_name, 128, "%s", line+8);
        else if (strncmp(line, "pin=", 4) == 0) cur->pinned = atoi(line+4);
    }
    fclose(f);
    return 0;
}

int snotes_save(const SessionNotes *sn, const char *path)
{
    FILE *f = fopen(path, "w");
    if (!f) return -1;
    for (int i = 0; i < sn->count; i++) {
        const SessionNote *n = &sn->notes[i];
        fprintf(f, "[note]\ntitle=%s\nbody=%s\nsession=%s\npin=%d\n\n",
                n->title, n->body, n->session_name, n->pinned);
    }
    fclose(f);
    return 0;
}
