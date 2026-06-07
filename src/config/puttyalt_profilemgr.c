/* puttyalt_profilemgr.c - Connection profile manager with inheritance. */
#include <string.h>
#include <stdio.h>
#define PM_MAX 32
#define PM_NAME 48
typedef struct { char name[PM_NAME]; int parent; int port; char user[48]; char keyfile[96]; int compress; int set_mask; } pm_profile;
typedef struct { pm_profile p[PM_MAX]; int n; } ProfileMgr;
#define PM_PORT 1
#define PM_USER 2
#define PM_KEY 4
#define PM_COMPRESS 8
void profilemgr_init(ProfileMgr *m) { if(m) memset(m,0,sizeof(*m)); }
int profilemgr_create(ProfileMgr *m, const char *name, int parent) {
    if(!m||m->n>=PM_MAX||!name) return -1;
    if (parent>=m->n) return -1;
    pm_profile *p=&m->p[m->n]; snprintf(p->name,PM_NAME,"%s",name); p->parent=parent>=0?parent:-1; return m->n++;
}
void profilemgr_set_port(ProfileMgr *m, int idx, int port) { if(m&&idx>=0&&idx<m->n){ m->p[idx].port=port; m->p[idx].set_mask|=PM_PORT; } }
void profilemgr_set_user(ProfileMgr *m, int idx, const char *user) { if(m&&idx>=0&&idx<m->n){ snprintf(m->p[idx].user,48,"%s",user?user:""); m->p[idx].set_mask|=PM_USER; } }
int profilemgr_effective_port(const ProfileMgr *m, int idx) {
    if(!m||idx<0||idx>=m->n) return 22;
    int cur=idx, guard=0;
    while (cur>=0 && guard++<PM_MAX) { if (m->p[cur].set_mask&PM_PORT) return m->p[cur].port; cur=m->p[cur].parent; }
    return 22;
}
int profilemgr_effective_user(const ProfileMgr *m, int idx, char *out, int outlen) {
    if(!m||idx<0||idx>=m->n) return -1;
    int cur=idx, guard=0;
    while (cur>=0 && guard++<PM_MAX) { if (m->p[cur].set_mask&PM_USER) { snprintf(out,outlen,"%s",m->p[cur].user); return 0; } cur=m->p[cur].parent; }
    out[0]=0; return -1;
}
int profilemgr_count(const ProfileMgr *m) { return m?m->n:-1; }
