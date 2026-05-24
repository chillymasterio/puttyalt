#ifndef PUTTYALT_SESSNOTES_H
#define PUTTYALT_SESSNOTES_H

#define SN_MAX_NOTES    256
#define SN_MAX_TITLE    128
#define SN_MAX_BODY     2048
#define SN_MAX_TAG      32
#define SN_MAX_TAGS     8

typedef struct {
    char  title[SN_MAX_TITLE];
    char  body[SN_MAX_BODY];
    char  tags[SN_MAX_TAGS][SN_MAX_TAG];
    int   tag_count;
    char  session_name[128];
    long  created;
    long  modified;
    int   pinned;
    int   color;
} SessionNote;

typedef struct {
    SessionNote notes[SN_MAX_NOTES];
    int         count;
} SessionNotes;

void snotes_init(SessionNotes *sn);
int  snotes_add(SessionNotes *sn, const char *title, const char *body, const char *session);
int  snotes_update(SessionNotes *sn, int index, const char *title, const char *body);
int  snotes_remove(SessionNotes *sn, int index);
int  snotes_add_tag(SessionNotes *sn, int index, const char *tag);
int  snotes_search(const SessionNotes *sn, const char *query, int *results, int max);
int  snotes_by_session(const SessionNotes *sn, const char *session, int *results, int max);
void snotes_pin(SessionNotes *sn, int index, int pin);
int  snotes_load(SessionNotes *sn, const char *path);
int  snotes_save(const SessionNotes *sn, const char *path);

#endif
