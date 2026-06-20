/* puttyalt_breadcrumb.c - Build navigation breadcrumb trails.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#include <stdio.h>
#define BC_MAX 16
typedef struct { char seg[BC_MAX][48]; int n; } Breadcrumb;
void bc3_init(Breadcrumb *b) { if (b) b->n = 0; }
int bc3_push(Breadcrumb *b, const char *seg) {
    if (!b || b->n >= BC_MAX || !seg) return -1;
    strncpy(b->seg[b->n], seg, 47); b->seg[b->n][47] = 0;
    b->n++;
    return 0;
}
void bc3_pop(Breadcrumb *b) { if (b && b->n > 0) b->n--; }
int bc3_render(const Breadcrumb *b, const char *sep, char *out, int outlen) {
    if (!b || !out) return -1;
    int o = 0;
    for (int i = 0; i < b->n; i++) {
        int n = snprintf(out + o, outlen - o, "%s%s", i ? sep : "", b->seg[i]);
        if (n < 0 || o + n >= outlen) return -1;
        o += n;
    }
    return o;
}
