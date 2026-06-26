/* puttyalt_portknock.c - Port-knocking sequence validation.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
#define PK_MAX 8
typedef struct { int sequence[PK_MAX]; int len; int progress; long last_knock; long timeout; } PortKnock;
void pk3_init(PortKnock *k, const int *seq, int len, long timeout) {
    if (!k) return;
    k->len = (len > 0 && len <= PK_MAX) ? len : 0;
    for (int i = 0; i < k->len; i++) k->sequence[i] = seq[i];
    k->progress = 0; k->last_knock = 0; k->timeout = timeout > 0 ? timeout : 10;
}
/* Process a knock on `port` at `now`. Returns 1 if sequence complete. */
int pk3_knock(PortKnock *k, int port, long now) {
    if (!k || k->len == 0) return 0;
    if (k->progress > 0 && now - k->last_knock > k->timeout) k->progress = 0;
    if (port == k->sequence[k->progress]) {
        k->progress++; k->last_knock = now;
        if (k->progress == k->len) { k->progress = 0; return 1; }
    } else {
        k->progress = (port == k->sequence[0]) ? 1 : 0;
        k->last_knock = now;
    }
    return 0;
}
