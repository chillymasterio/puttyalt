/* puttyalt_configwatch.c - Track config file change detection.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stdint.h>
#include <string.h>
typedef struct { uint32_t last_hash; long last_check; int changed; } ConfigWatch;
void cw3_init(ConfigWatch *w) { if (w) { w->last_hash = 0; w->last_check = 0; w->changed = 0; } }
static uint32_t cw3_hash(const char *content, int n) {
    uint32_t h = 2166136261u;
    for (int i = 0; i < n; i++) { h ^= (unsigned char)content[i]; h *= 16777619u; }
    return h;
}
/* Check content; returns 1 if changed since last check. */
int cw3_check(ConfigWatch *w, const char *content, int n, long now) {
    if (!w || !content) return 0;
    uint32_t h = cw3_hash(content, n);
    w->last_check = now;
    if (w->last_hash != 0 && h != w->last_hash) { w->changed = 1; w->last_hash = h; return 1; }
    w->last_hash = h;
    return 0;
}
