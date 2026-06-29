/* puttyalt_sessionnotes.c - Attach and search session notes.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#define SN_MAX 32
typedef struct { int session_id; char note[120]; } SnNote;
typedef struct { SnNote notes[SN_MAX]; int n; } SessionNotes;
void sn4_init(SessionNotes *s) { if (s) s->n = 0; }
int sn4_set(SessionNotes *s, int session_id, const char *note) {
    if (!s || !note) return -1;
    for (int i = 0; i < s->n; i++) if (s->notes[i].session_id == session_id) {
        strncpy(s->notes[i].note, note, 119); s->notes[i].note[119] = 0; return 0;
    }
    if (s->n >= SN_MAX) return -1;
    s->notes[s->n].session_id = session_id;
    strncpy(s->notes[s->n].note, note, 119); s->notes[s->n].note[119] = 0;
    s->n++;
    return 0;
}
const char *sn4_get(const SessionNotes *s, int session_id) {
    if (!s) return 0;
    for (int i = 0; i < s->n; i++) if (s->notes[i].session_id == session_id) return s->notes[i].note;
    return 0;
}
int sn4_search(const SessionNotes *s, const char *term, int *out, int cap) {
    if (!s || !term) return 0;
    int c = 0;
    for (int i = 0; i < s->n && c < cap; i++) if (strstr(s->notes[i].note, term)) out[c++] = s->notes[i].session_id;
    return c;
}
