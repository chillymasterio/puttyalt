/* puttyalt_statusseg.c - Status-bar segment layout (left/center/right with priority drop). */
#include <string.h>
#include <stdio.h>
#define SS_MAX 24
#define SS_TEXT 64
enum ss_align { SS_LEFT=0, SS_CENTER=1, SS_RIGHT=2 };
typedef struct { char text[SS_TEXT]; int align; int priority; int visible; } ss_seg;
typedef struct { ss_seg seg[SS_MAX]; int n; int width; } StatusSeg;
void statusseg_init(StatusSeg *s, int width) { if(!s) return; memset(s,0,sizeof(*s)); s->width=width; }
int statusseg_add(StatusSeg *s, const char *text, int align, int priority) {
    if(!s||s->n>=SS_MAX) return -1;
    ss_seg *g=&s->seg[s->n]; snprintf(g->text,SS_TEXT,"%s",text?text:""); g->align=align; g->priority=priority; g->visible=1;
    return s->n++;
}
int statusseg_layout(StatusSeg *s) {
    if(!s) return -1;
    /* total width; drop lowest-priority segments until it fits */
    for (int i=0;i<s->n;i++) s->seg[i].visible=1;
    int used; int guard=0;
    do {
        used=0; for (int i=0;i<s->n;i++) if (s->seg[i].visible) used+=(int)strlen(s->seg[i].text)+1;
        if (used<=s->width) break;
        int lowest=-1; for (int i=0;i<s->n;i++) if (s->seg[i].visible && (lowest<0||s->seg[i].priority<s->seg[lowest].priority)) lowest=i;
        if (lowest<0) break;
        s->seg[lowest].visible=0;
    } while (++guard<SS_MAX);
    return used;
}
int statusseg_visible_count(const StatusSeg *s) { if(!s) return -1; int n=0; for(int i=0;i<s->n;i++) if(s->seg[i].visible)n++; return n; }
