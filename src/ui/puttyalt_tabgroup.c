#include <string.h>
#include <stdio.h>
#define MAX_GROUPS 32
#define MAX_TABS_PER_GROUP 16
#define GROUP_NAME_LEN 64
typedef struct { char name[GROUP_NAME_LEN]; int tab_ids[MAX_TABS_PER_GROUP]; int count; unsigned int color; } TabGroup;
static TabGroup g_groups[MAX_GROUPS]; static int g_group_count = 0;
int tabgroup_create(const char *name, unsigned int color) {
    if (g_group_count >= MAX_GROUPS) return -1;
    TabGroup *g = &g_groups[g_group_count]; memset(g, 0, sizeof(*g));
    snprintf(g->name, GROUP_NAME_LEN, "%s", name); g->color = color;
    return g_group_count++;
}
int tabgroup_add_tab(int group_id, int tab_id) {
    if (group_id < 0 || group_id >= g_group_count) return -1;
    TabGroup *g = &g_groups[group_id];
    if (g->count >= MAX_TABS_PER_GROUP) return -1;
    g->tab_ids[g->count++] = tab_id; return 0;
}
int tabgroup_remove_tab(int group_id, int tab_id) {
    if (group_id < 0 || group_id >= g_group_count) return -1;
    TabGroup *g = &g_groups[group_id];
    for (int i = 0; i < g->count; i++) if (g->tab_ids[i] == tab_id) {
        memmove(&g->tab_ids[i], &g->tab_ids[i+1], sizeof(int)*(g->count-i-1));
        g->count--; return 0;
    }
    return -1;
}
int tabgroup_count(void) { return g_group_count; }
