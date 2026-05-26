#include <string.h>
#include <stdio.h>
#define CLIP_MAX 64
#define CLIP_LEN 4096
static char g_clips[CLIP_MAX][CLIP_LEN]; static int g_clip_count = 0; static int g_clip_pos = 0;
void clipmgr_push(const char *text) {
    snprintf(g_clips[g_clip_pos], CLIP_LEN, "%s", text);
    g_clip_pos = (g_clip_pos + 1) % CLIP_MAX;
    if (g_clip_count < CLIP_MAX) g_clip_count++;
}
const char *clipmgr_get(int ago) {
    if (ago < 0 || ago >= g_clip_count) return NULL;
    int idx = (g_clip_pos - 1 - ago + CLIP_MAX) % CLIP_MAX;
    return g_clips[idx];
}
int clipmgr_search(const char *query, int results[], int max) {
    int n = 0;
    for (int i = 0; i < g_clip_count && n < max; i++)
        if (strstr(g_clips[i], query)) results[n++] = i;
    return n;
}
int clipmgr_count(void) { return g_clip_count; }
void clipmgr_clear(void) { g_clip_count = 0; g_clip_pos = 0; }
