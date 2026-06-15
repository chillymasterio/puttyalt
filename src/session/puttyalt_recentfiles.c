/* puttyalt_recentfiles.c - Track recently opened files (MRU).
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#define RF_CAP 16
#define RF_LEN 128
typedef struct { char path[RF_CAP][RF_LEN]; int n; } RecentFiles;
void rf2_init(RecentFiles *r) { if (r) r->n = 0; }
void rf2_add(RecentFiles *r, const char *path) {
    if (!r || !path) return;
    /* remove existing */
    for (int i = 0; i < r->n; i++)
        if (strcmp(r->path[i], path) == 0) {
            for (int j = i; j < r->n - 1; j++) memcpy(r->path[j], r->path[j+1], RF_LEN);
            r->n--; break;
        }
    /* shift down, insert at front */
    if (r->n >= RF_CAP) r->n = RF_CAP - 1;
    for (int j = r->n; j > 0; j--) memcpy(r->path[j], r->path[j-1], RF_LEN);
    strncpy(r->path[0], path, RF_LEN-1); r->path[0][RF_LEN-1] = 0;
    if (r->n < RF_CAP) r->n++;
}
const char *rf2_get(const RecentFiles *r, int i) {
    if (!r || i < 0 || i >= r->n) return 0;
    return r->path[i];
}
