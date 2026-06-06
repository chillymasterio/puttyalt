/* puttyalt_rbac.c - Role-based access control matrix (roles x permissions). */
#include <string.h>
#include <stdio.h>
#define RB_ROLES 16
#define RB_NAME 32
typedef struct { char name[RB_NAME]; int perm_mask; int inherits; } rb_role;
typedef struct { rb_role roles[RB_ROLES]; int n; } Rbac;
void rbac_init(Rbac *r) { if(r) memset(r,0,sizeof(*r)); }
int rbac_add_role(Rbac *r, const char *name, int perm_mask, int inherits) {
    if(!r||r->n>=RB_ROLES||!name) return -1;
    rb_role *role=&r->roles[r->n]; snprintf(role->name,RB_NAME,"%s",name); role->perm_mask=perm_mask;
    role->inherits = (inherits>=0 && inherits<r->n) ? inherits : -1; return r->n++;
}
int rbac_effective_perms(const Rbac *r, int role_idx) {
    if(!r||role_idx<0||role_idx>=r->n) return 0;
    int perms=r->roles[role_idx].perm_mask;
    int cur=r->roles[role_idx].inherits, guard=0;
    while (cur>=0 && guard++<RB_ROLES) { perms|=r->roles[cur].perm_mask; cur=r->roles[cur].inherits; }
    return perms;
}
int rbac_can(const Rbac *r, int role_idx, int permission_bit) {
    return (rbac_effective_perms(r,role_idx) & permission_bit)?1:0;
}
int rbac_find_role(const Rbac *r, const char *name) {
    if(!r||!name) return -1;
    for (int i=0;i<r->n;i++) if (strcmp(r->roles[i].name,name)==0) return i;
    return -1;
}
int rbac_count(const Rbac *r) { return r?r->n:-1; }
