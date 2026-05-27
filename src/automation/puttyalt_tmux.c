#include <string.h>
#include <stdio.h>
#define MAX_TMUX_PANES 16
typedef struct { int id; char title[128]; int active; int width; int height; } TmuxPane;
typedef struct { int session_id; char name[128]; TmuxPane panes[MAX_TMUX_PANES]; int pane_count; } TmuxSession;
static TmuxSession g_tmux; static int g_tmux_active = 0;
void tmux_init(void) { memset(&g_tmux,0,sizeof(g_tmux)); }
int tmux_detect(const char *output) {
    return strstr(output, "\x1bP1000p") != NULL || strstr(output, "tmux") != NULL;
}
int tmux_parse_pane_info(const char *info, TmuxPane *pane) {
    return sscanf(info, "%d:%127[^:]:%dx%d", &pane->id, pane->title, &pane->width, &pane->height);
}
int tmux_build_cmd(const char *subcmd, char *buf, int buflen) {
    return snprintf(buf, buflen, "tmux %s", subcmd);
}
int tmux_is_active(void) { return g_tmux_active; }
void tmux_set_active(int active) { g_tmux_active = active; }
int tmux_pane_count(void) { return g_tmux.pane_count; }
