#ifndef PUTTYALT_SESSNOTE_H
#define PUTTYALT_SESSNOTE_H

#define NOTE_MAX_LEN 4096

typedef struct {
    char session_name[128];
    char text[NOTE_MAX_LEN];
    char last_modified[32];
    int  show_on_connect;
    int  pinned;
} SessionNote;

typedef struct {
    SessionNote *notes;
    int count;
    int capacity;
} NoteStore;

void notestore_init(NoteStore *ns);
void notestore_free(NoteStore *ns);
int  notestore_add(NoteStore *ns, const char *session, const char *text);
int  notestore_update(NoteStore *ns, const char *session, const char *text);
SessionNote *notestore_find(NoteStore *ns, const char *session);
int  notestore_remove(NoteStore *ns, const char *session);
int  notestore_save(NoteStore *ns, const char *path);
int  notestore_load(NoteStore *ns, const char *path);

#endif
