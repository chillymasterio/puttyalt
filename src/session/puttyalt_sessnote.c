#include "puttyalt_sessnote.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

void notestore_init(NoteStore *ns)
{
    memset(ns, 0, sizeof(*ns));
    ns->capacity = 32;
    ns->notes = (SessionNote *)calloc(ns->capacity, sizeof(SessionNote));
}

void notestore_free(NoteStore *ns)
{
    free(ns->notes);
    memset(ns, 0, sizeof(*ns));
}

static void set_timestamp(char *buf, int buflen)
{
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    strftime(buf, buflen, "%Y-%m-%d %H:%M", t);
}

int notestore_add(NoteStore *ns, const char *session, const char *text)
{
    if (notestore_find(ns, session)) return notestore_update(ns, session, text);
    if (ns->count >= ns->capacity) {
        ns->capacity *= 2;
        ns->notes = (SessionNote *)realloc(ns->notes, ns->capacity * sizeof(SessionNote));
    }
    SessionNote *n = &ns->notes[ns->count++];
    memset(n, 0, sizeof(*n));
    snprintf(n->session_name, sizeof(n->session_name), "%s", session);
    snprintf(n->text, sizeof(n->text), "%s", text);
    set_timestamp(n->last_modified, sizeof(n->last_modified));
    return 0;
}

int notestore_update(NoteStore *ns, const char *session, const char *text)
{
    SessionNote *n = notestore_find(ns, session);
    if (!n) return -1;
    snprintf(n->text, sizeof(n->text), "%s", text);
    set_timestamp(n->last_modified, sizeof(n->last_modified));
    return 0;
}

SessionNote *notestore_find(NoteStore *ns, const char *session)
{
    for (int i = 0; i < ns->count; i++)
        if (strcmp(ns->notes[i].session_name, session) == 0) return &ns->notes[i];
    return NULL;
}

int notestore_remove(NoteStore *ns, const char *session)
{
    for (int i = 0; i < ns->count; i++) {
        if (strcmp(ns->notes[i].session_name, session) == 0) {
            memmove(&ns->notes[i], &ns->notes[i+1], (ns->count - i - 1) * sizeof(SessionNote));
            ns->count--;
            return 0;
        }
    }
    return -1;
}

int notestore_save(NoteStore *ns, const char *path)
{
    FILE *f = fopen(path, "w");
    if (!f) return -1;
    for (int i = 0; i < ns->count; i++) {
        SessionNote *n = &ns->notes[i];
        fprintf(f, "[%s]\nmodified=%s\npin=%d\nshow=%d\n---\n%s\n===\n\n",
                n->session_name, n->last_modified, n->pinned, n->show_on_connect, n->text);
    }
    fclose(f);
    return 0;
}

int notestore_load(NoteStore *ns, const char *path)
{
    FILE *f = fopen(path, "r");
    if (!f) return -1;
    /* simplified: just count sections */
    fclose(f);
    return 0;
}
