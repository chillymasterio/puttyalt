#include <string.h>
#include <stdio.h>
#include <time.h>
#define MAX_WATCHES 32
typedef struct { char path[512]; long last_modified; long last_size; int active; void (*callback)(const char*); } FileWatch;
static FileWatch g_watches[MAX_WATCHES]; static int g_watch_count = 0;
int filewatch_add(const char *path, void (*cb)(const char*)) {
    if (g_watch_count >= MAX_WATCHES) return -1;
    FileWatch *w = &g_watches[g_watch_count]; memset(w, 0, sizeof(*w));
    snprintf(w->path, 512, "%s", path); w->callback = cb; w->active = 1;
    return g_watch_count++;
}
int filewatch_remove(int idx) {
    if (idx < 0 || idx >= g_watch_count) return -1;
    g_watches[idx].active = 0; return 0;
}
int filewatch_count(void) { return g_watch_count; }
