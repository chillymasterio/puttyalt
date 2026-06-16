/* puttyalt_macrorec.c - Record and replay keystroke macros.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
#define MREC_CAP 256
typedef struct { int keys[MREC_CAP]; int n; int recording; } MacroRec;
void mrec_init(MacroRec *m) { if (m) { m->n = 0; m->recording = 0; } }
void mrec_start(MacroRec *m) { if (m) { m->n = 0; m->recording = 1; } }
void mrec_stop(MacroRec *m) { if (m) m->recording = 0; }
int mrec_key(MacroRec *m, int key) {
    if (!m || !m->recording) return -1;
    if (m->n >= MREC_CAP) return -1;
    m->keys[m->n++] = key;
    return 0;
}
int mrec_replay(const MacroRec *m, int *out, int outcap) {
    if (!m) return 0;
    int n = m->n < outcap ? m->n : outcap;
    for (int i = 0; i < n; i++) out[i] = m->keys[i];
    return n;
}
