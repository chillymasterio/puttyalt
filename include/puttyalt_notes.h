/*
 * puttyalt_notes.h: Per-session notes and annotations.
 *
 * Attach free-text notes to sessions for documenting server
 * purpose, credentials location, or maintenance schedules.
 */

#ifndef PUTTYALT_NOTES_H
#define PUTTYALT_NOTES_H

#define MAX_NOTE_LENGTH 4096

typedef struct SessionNote {
    char session_name[256];
    char text[MAX_NOTE_LENGTH];
    long created_at;
    long modified_at;
} SessionNote;

int notes_load(SessionNote *note, const char *session, const char *config_dir);
int notes_save(const SessionNote *note, const char *config_dir);
int notes_delete(const char *session, const char *config_dir);

#endif /* PUTTYALT_NOTES_H */
