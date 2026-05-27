#include <string.h>
#include <stdio.h>
#define STATUS_MAX_SEGMENTS 8
#define SEG_LEN 128
typedef struct { char text[SEG_LEN]; unsigned int color; int align; } StatusSegment;
static StatusSegment g_segments[STATUS_MAX_SEGMENTS]; static int g_seg_count = 0;
void statusline_clear(void) { g_seg_count = 0; }
int statusline_add(const char *text, unsigned int color, int align) {
    if (g_seg_count >= STATUS_MAX_SEGMENTS) return -1;
    StatusSegment *s = &g_segments[g_seg_count]; memset(s, 0, sizeof(*s));
    snprintf(s->text, SEG_LEN, "%s", text); s->color = color; s->align = align;
    return g_seg_count++;
}
void statusline_update(int idx, const char *text) {
    if (idx >= 0 && idx < g_seg_count) snprintf(g_segments[idx].text, SEG_LEN, "%s", text);
}
int statusline_format(char *buf, int buflen) {
    int pos = 0;
    for (int i = 0; i < g_seg_count && pos < buflen - SEG_LEN; i++)
        pos += snprintf(buf + pos, buflen - pos, "%s%s", i > 0 ? " | " : "", g_segments[i].text);
    return pos;
}
int statusline_count(void) { return g_seg_count; }
