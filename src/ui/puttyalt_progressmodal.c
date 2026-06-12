/* puttyalt_progressmodal.c - Progress modal for long operations (transfer/scan). */
#include <string.h>
#include <stdio.h>
#include <stdint.h>
typedef struct { char title[64]; char detail[128]; int percent; int cancellable; int cancelled; int visible; uint64_t start_ms; } ProgressModal;
void progressmodal_init(ProgressModal *p) { if(p) memset(p,0,sizeof(*p)); }
int progressmodal_show(ProgressModal *p, const char *title, int cancellable, uint64_t now_ms) {
    if(!p) return -1;
    snprintf(p->title,64,"%s",title?title:""); p->cancellable=cancellable?1:0;
    p->visible=1; p->percent=0; p->cancelled=0; p->start_ms=now_ms; return 0;
}
void progressmodal_update(ProgressModal *p, int percent, const char *detail) {
    if(!p) return;
    p->percent=percent<0?0:(percent>100?100:percent);
    if (detail) snprintf(p->detail,128,"%s",detail);
}
int progressmodal_cancel(ProgressModal *p) {
    if(!p||!p->cancellable) return -1;
    p->cancelled=1; return 0;
}
int progressmodal_eta(const ProgressModal *p, uint64_t now_ms) {
    if(!p||p->percent<=0) return -1;
    uint64_t elapsed=now_ms-p->start_ms;
    return (int)((elapsed*(100-p->percent)/p->percent)/1000);
}
void progressmodal_hide(ProgressModal *p) { if(p) p->visible=0; }
int progressmodal_is_cancelled(const ProgressModal *p) { return p?p->cancelled:-1; }
int progressmodal_is_visible(const ProgressModal *p) { return p?p->visible:-1; }
