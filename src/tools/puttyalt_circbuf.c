/* puttyalt_circbuf.c - Circular byte buffer (ring) with overflow.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#define CB_SIZE 1024
typedef struct { unsigned char buf[CB_SIZE]; int head, tail, count; } CircBuf;
void cb2_init(CircBuf *c) { if (c) { c->head = c->tail = c->count = 0; } }
int cb2_push(CircBuf *c, unsigned char b) {
    if (!c || c->count >= CB_SIZE) return -1;
    c->buf[c->head] = b;
    c->head = (c->head + 1) % CB_SIZE;
    c->count++;
    return 0;
}
int cb2_pop(CircBuf *c, unsigned char *out) {
    if (!c || c->count == 0) return -1;
    if (out) *out = c->buf[c->tail];
    c->tail = (c->tail + 1) % CB_SIZE;
    c->count--;
    return 0;
}
int cb2_available(const CircBuf *c) { return c ? c->count : 0; }
int cb2_free(const CircBuf *c) { return c ? CB_SIZE - c->count : 0; }
