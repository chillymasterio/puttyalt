/* puttyalt_cfgmigrate2.c - Config migration v2 with version chain. */
#include <string.h>
#include <stdio.h>
#define CM_MAX 16
typedef struct { int from_version; int to_version; char description[64]; int applied; } cm_migration;
typedef struct { cm_migration migrations[CM_MAX]; int n; int current_version; int target_version; } CfgMigrate2;
void cfgmigrate2_init(CfgMigrate2 *c, int current, int target) {
    if(!c) return; memset(c,0,sizeof(*c)); c->current_version=current; c->target_version=target;
}
int cfgmigrate2_register(CfgMigrate2 *c, int from, int to, const char *desc) {
    if(!c||c->n>=CM_MAX||to!=from+1) return -1;
    cm_migration *m=&c->migrations[c->n++]; m->from_version=from; m->to_version=to;
    snprintf(m->description,64,"%s",desc?desc:""); return 0;
}
int cfgmigrate2_next(CfgMigrate2 *c) {
    if(!c||c->current_version>=c->target_version) return -1;
    for (int i=0;i<c->n;i++) if (c->migrations[i].from_version==c->current_version && !c->migrations[i].applied) {
        c->migrations[i].applied=1; c->current_version=c->migrations[i].to_version; return i;
    }
    return -1; /* gap in migration chain */
}
int cfgmigrate2_is_complete(const CfgMigrate2 *c) { return (c&&c->current_version>=c->target_version)?1:0; }
int cfgmigrate2_pending(const CfgMigrate2 *c) { return c?(c->target_version-c->current_version):-1; }
