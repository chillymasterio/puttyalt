/* puttyalt_migstep.c - Versioned config migration step runner. */
#include <string.h>
#include <stdio.h>
#define MS_MAX 32
typedef struct { int from_ver, to_ver; char desc[64]; int applied; } mig_step;
typedef struct { mig_step s[MS_MAX]; int n; int current_ver; } MigStep;
void migstep_init(MigStep *m, int current_ver) { if(m){ memset(m,0,sizeof(*m)); m->current_ver=current_ver; } }
int migstep_register(MigStep *m, int from_ver, int to_ver, const char *desc) {
    if(!m||m->n>=MS_MAX||to_ver!=from_ver+1) return -1;
    mig_step *s=&m->s[m->n++]; s->from_ver=from_ver; s->to_ver=to_ver; snprintf(s->desc,64,"%s",desc?desc:""); return 0;
}
int migstep_run_to(MigStep *m, int target_ver) {
    if(!m) return -1;
    int applied=0;
    while (m->current_ver<target_ver) {
        int found=0;
        for (int i=0;i<m->n;i++) if (m->s[i].from_ver==m->current_ver && !m->s[i].applied) {
            m->s[i].applied=1; m->current_ver=m->s[i].to_ver; applied++; found=1; break;
        }
        if (!found) return -1;
        /* gap in migration chain */
    }
    return applied;
}
int migstep_current(const MigStep *m) { return m?m->current_ver:-1; }
