/* puttyalt_featureflags.c - Feature flag registry with rollout percentage gating. */
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#define FF_MAX 48
#define FF_NAME 48
typedef struct { char name[FF_NAME]; int enabled; int rollout_pct; } ff_flag;
typedef struct { ff_flag f[FF_MAX]; int n; uint32_t user_hash; } FeatureFlags;
void featureflags_init(FeatureFlags *f, uint32_t user_hash) { if(f){ memset(f,0,sizeof(*f)); f->user_hash=user_hash; } }
int featureflags_register(FeatureFlags *f, const char *name, int enabled, int rollout_pct) {
    if(!f||f->n>=FF_MAX||!name) return -1;
    ff_flag *fl=&f->f[f->n++]; snprintf(fl->name,FF_NAME,"%s",name); fl->enabled=enabled?1:0;
    fl->rollout_pct = rollout_pct<0?0:(rollout_pct>100?100:rollout_pct); return 0;
}
int featureflags_is_on(const FeatureFlags *f, const char *name) {
    if(!f||!name) return 0;
    for (int i=0;i<f->n;i++) if (strcmp(f->f[i].name,name)==0) {
        if (!f->f[i].enabled) return 0;
        if (f->f[i].rollout_pct>=100) return 1;
        /* deterministic per-user bucket */
        uint32_t h=f->user_hash;
        for (const char *p=name;*p;p++) { h^=(uint8_t)*p; h*=16777619u; }
        return ((h%100) < (uint32_t)f->f[i].rollout_pct) ? 1 : 0;
    }
    return 0;
}
int featureflags_set(FeatureFlags *f, const char *name, int enabled) {
    if(!f||!name) return -1;
    for (int i=0;i<f->n;i++) if (strcmp(f->f[i].name,name)==0) { f->f[i].enabled=enabled?1:0; return 0; }
    return -1;
}
int featureflags_count(const FeatureFlags *f) { return f?f->n:-1; }
