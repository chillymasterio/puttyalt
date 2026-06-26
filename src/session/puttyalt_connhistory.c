/* puttyalt_connhistory.c - Record connection history entries.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#define CH_MAX 64
typedef struct { char host[64]; long connected_at; int success; } ChEntry;
typedef struct { ChEntry e[CH_MAX]; int n; int head; } ConnHistory;
void ch3_init(ConnHistory *c) { if (c) { c->n = 0; c->head = 0; } }
void ch3_record(ConnHistory *c, const char *host, long when, int success) {
    if (!c || !host) return;
    strncpy(c->e[c->head].host, host, 63); c->e[c->head].host[63] = 0;
    c->e[c->head].connected_at = when; c->e[c->head].success = success;
    c->head = (c->head + 1) % CH_MAX;
    if (c->n < CH_MAX) c->n++;
}
int ch3_success_rate(const ConnHistory *c, const char *host) {
    if (!c || !host) return 0;
    int total = 0, ok = 0;
    for (int i = 0; i < c->n; i++) if (strcmp(c->e[i].host, host) == 0) { total++; if (c->e[i].success) ok++; }
    return total ? ok * 100 / total : 0;
}
