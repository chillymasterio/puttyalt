/* puttyalt_a11y.c - Accessibility: screen-reader announce queue + focus roles. */
#include <string.h>
#include <stdio.h>
#define SRQ_MAX_MSGS 32
#define SRQ_MSG_LEN 256
enum srq_priority { SRQ_LOW=0, SRQ_NORMAL=1, SRQ_HIGH=2, SRQ_ASSERTIVE=3 };
enum srq_role { SRQ_ROLE_NONE=0, SRQ_ROLE_BUTTON, SRQ_ROLE_TAB, SRQ_ROLE_TEXTBOX, SRQ_ROLE_TERMINAL, SRQ_ROLE_LIST, SRQ_ROLE_MENU };
typedef struct { char text[SRQ_MSG_LEN]; int priority; } srq_msg;
typedef struct { srq_msg q[SRQ_MAX_MSGS]; int head, tail, count; int enabled; int focus_role; char focus_label[128]; } SRQueue;
void srq_init(SRQueue *a) { if(!a) return; memset(a,0,sizeof(*a)); a->enabled=1; }
void srq_set_enabled(SRQueue *a, int on) { if(a) a->enabled=on?1:0; }
int srq_announce(SRQueue *a, const char *text, int priority) {
    if (!a || !text || !a->enabled) return -1;
    if (a->count>=SRQ_MAX_MSGS) { a->head=(a->head+1)%SRQ_MAX_MSGS; a->count--; } /* drop oldest */
    srq_msg *m=&a->q[a->tail]; snprintf(m->text,SRQ_MSG_LEN,"%s",text);
    m->priority = priority<SRQ_LOW?SRQ_LOW:(priority>SRQ_ASSERTIVE?SRQ_ASSERTIVE:priority);
    a->tail=(a->tail+1)%SRQ_MAX_MSGS; a->count++; return 0;
}
int srq_next(SRQueue *a, char *buf, int buflen, int *out_priority) {
    if (!a || a->count==0) return -1;
    srq_msg *m=&a->q[a->head];
    if (buf) snprintf(buf,buflen,"%s",m->text);
    if (out_priority) *out_priority=m->priority;
    a->head=(a->head+1)%SRQ_MAX_MSGS; a->count--; return 0;
}
void srq_set_focus(SRQueue *a, int role, const char *label) {
    if(!a) return; a->focus_role=role; snprintf(a->focus_label,128,"%s",label?label:"");
}
int srq_pending(const SRQueue *a) { return a?a->count:-1; }
