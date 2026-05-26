#include <string.h>
#include <stdio.h>
#define MAX_NOTES 256
#define NOTE_LEN 2048
typedef struct { char session[128]; char note[NOTE_LEN]; long timestamp; } SessionNote;
static SessionNote g_notes[MAX_NOTES]; static int g_note_count = 0;
int sessnotes_add(const char *session, const char *note, long ts) {
    if (g_note_count >= MAX_NOTES) return -1;
    SessionNote *n = &g_notes[g_note_count]; memset(n, 0, sizeof(*n));
    snprintf(n->session, 128, "%s", session); snprintf(n->note, NOTE_LEN, "%s", note);
    n->timestamp = ts; g_note_count++; return 0;
}
int sessnotes_get(const char *session, char notes[][NOTE_LEN], int max) {
    int n = 0;
    for (int i = g_note_count-1; i >= 0 && n < max; i--)
        if (strcmp(g_notes[i].session, session) == 0)
            snprintf(notes[n++], NOTE_LEN, "%s", g_notes[i].note);
    return n;
}
int sessnotes_count(const char *session) {
    int n = 0;
    for (int i = 0; i < g_note_count; i++) if (strcmp(g_notes[i].session, session) == 0) n++;
    return n;
}
