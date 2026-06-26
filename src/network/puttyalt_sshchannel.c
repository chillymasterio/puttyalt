/* puttyalt_sshchannel.c - Track SSH channel multiplexing state.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
#define SC_MAX 16
typedef struct { int local_id; int remote_id; int window; int open; } ScChannel;
typedef struct { ScChannel ch[SC_MAX]; int next_id; } SshChannels;
void sc2_init(SshChannels *s) { if (s) { for (int i=0;i<SC_MAX;i++) s->ch[i].open = 0; s->next_id = 100; } }
int sc2_open(SshChannels *s, int window) {
    if (!s) return -1;
    for (int i = 0; i < SC_MAX; i++) if (!s->ch[i].open) {
        s->ch[i].local_id = s->next_id++; s->ch[i].remote_id = -1;
        s->ch[i].window = window; s->ch[i].open = 1;
        return s->ch[i].local_id;
    }
    return -1;
}
int sc2_consume_window(SshChannels *s, int local_id, int bytes) {
    if (!s) return -1;
    for (int i = 0; i < SC_MAX; i++) if (s->ch[i].open && s->ch[i].local_id == local_id) {
        if (s->ch[i].window < bytes) return -1;
        s->ch[i].window -= bytes;
        return s->ch[i].window;
    }
    return -1;
}
int sc2_close(SshChannels *s, int local_id) {
    if (!s) return -1;
    for (int i = 0; i < SC_MAX; i++) if (s->ch[i].open && s->ch[i].local_id == local_id) { s->ch[i].open = 0; return 0; }
    return -1;
}
int sc2_active(const SshChannels *s) {
    if (!s) return 0;
    int c = 0;
    for (int i = 0; i < SC_MAX; i++) if (s->ch[i].open) c++;
    return c;
}
