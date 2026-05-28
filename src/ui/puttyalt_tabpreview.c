#include <string.h>
#include <stdio.h>
#define MAX_PREVIEW_LINES 8
#define PREVIEW_LINE_LEN 120
typedef struct { int tab_id; char title[128]; char lines[MAX_PREVIEW_LINES][PREVIEW_LINE_LEN]; int line_count; int active; } TabPreview;
#define MAX_PREVIEWS 32
static TabPreview g_previews[MAX_PREVIEWS]; static int g_preview_count = 0;
int tabpreview_update(int tab_id, const char *title, const char **lines, int line_count) {
    TabPreview *p = NULL;
    for (int i=0;i<g_preview_count;i++) if (g_previews[i].tab_id==tab_id) { p=&g_previews[i]; break; }
    if (!p) { if (g_preview_count>=MAX_PREVIEWS) return -1; p=&g_previews[g_preview_count++]; }
    p->tab_id = tab_id; snprintf(p->title, 128, "%s", title);
    p->line_count = line_count < MAX_PREVIEW_LINES ? line_count : MAX_PREVIEW_LINES;
    for (int i=0;i<p->line_count;i++) snprintf(p->lines[i], PREVIEW_LINE_LEN, "%s", lines[i]);
    return 0;
}
TabPreview *tabpreview_get(int tab_id) {
    for (int i=0;i<g_preview_count;i++) if (g_previews[i].tab_id==tab_id) return &g_previews[i];
    return NULL;
}
int tabpreview_count(void) { return g_preview_count; }
