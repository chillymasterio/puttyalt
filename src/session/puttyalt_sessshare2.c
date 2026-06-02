/* puttyalt_sessshare2.c - Shared session participant roster with roles. */
#include <string.h>
#include <stdio.h>
#define SS_MAX 16
#define SS_NAME 48
enum ss_role { SS_VIEWER=0, SS_COMMENTER=1, SS_DRIVER=2, SS_OWNER=3 };
typedef struct { char name[SS_NAME]; int role; int active; } ss_part;
typedef struct { ss_part p[SS_MAX]; int n; int driver_idx; } SessShare2;
void sessshare2_init(SessShare2 *s) { if(s){ memset(s,0,sizeof(*s)); s->driver_idx=-1; } }
int sessshare2_join(SessShare2 *s, const char *name, int role) {
    if(!s||s->n>=SS_MAX||!name) return -1;
    ss_part *p=&s->p[s->n]; snprintf(p->name,SS_NAME,"%s",name); p->role=role; p->active=1;
    if (role==SS_DRIVER || role==SS_OWNER) s->driver_idx=s->n;
    return s->n++;
}
int sessshare2_leave(SessShare2 *s, const char *name) {
    if(!s||!name) return -1;
    for (int i=0;i<s->n;i++) if (strcmp(s->p[i].name,name)==0) { s->p[i].active=0; if(s->driver_idx==i)s->driver_idx=-1; return 0; }
    return -1;
}
int sessshare2_pass_driver(SessShare2 *s, const char *name) {
    if(!s||!name) return -1;
    for (int i=0;i<s->n;i++) if (strcmp(s->p[i].name,name)==0 && s->p[i].active) {
        if (s->driver_idx>=0) s->p[s->driver_idx].role=SS_VIEWER;
        s->p[i].role=SS_DRIVER; s->driver_idx=i; return 0;
    }
    return -1;
}
int sessshare2_can_input(const SessShare2 *s, const char *name) {
    if(!s||!name) return 0;
    for (int i=0;i<s->n;i++) if (strcmp(s->p[i].name,name)==0) return (s->p[i].role>=SS_DRIVER && s->p[i].active)?1:0;
    return 0;
}
int sessshare2_active_count(const SessShare2 *s) { if(!s) return -1; int n=0; for(int i=0;i<s->n;i++) if(s->p[i].active)n++; return n; }
