#include <string.h>
#include <stdio.h>
#define MAX_RECENT 16
static int g_recent[MAX_RECENT]; static int g_recent_count = 0; static int g_recent_pos = 0;
void quickswitch_push(int tab_id) {
    for (int i=0;i<g_recent_count;i++) if (g_recent[i]==tab_id) {
        memmove(&g_recent[i],&g_recent[i+1],sizeof(int)*(g_recent_count-i-1)); g_recent_count--; break;
    }
    if (g_recent_count >= MAX_RECENT) { memmove(&g_recent[0],&g_recent[1],sizeof(int)*(MAX_RECENT-1)); g_recent_count--; }
    g_recent[g_recent_count++] = tab_id;
}
int quickswitch_prev(void) { g_recent_pos++; if (g_recent_pos >= g_recent_count) g_recent_pos = 0; return g_recent_count > 0 ? g_recent[g_recent_count - 1 - g_recent_pos] : -1; }
int quickswitch_next(void) { g_recent_pos--; if (g_recent_pos < 0) g_recent_pos = g_recent_count - 1; return g_recent_count > 0 ? g_recent[g_recent_count - 1 - g_recent_pos] : -1; }
void quickswitch_reset_pos(void) { g_recent_pos = 0; }
int quickswitch_count(void) { return g_recent_count; }
