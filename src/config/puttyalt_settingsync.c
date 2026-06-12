/* puttyalt_settingsync.c - Settings sync orchestrator: diff -> push/pull decisions. */
#include <string.h>
#include <stdint.h>
enum ss_action { SS_NOOP=0, SS_PUSH=1, SS_PULL=2, SS_CONFLICT=3 };
typedef struct { uint64_t local_rev, remote_rev, base_rev; int local_dirty; } SettingSync;
void settingsync_init(SettingSync *s) { if(s) memset(s,0,sizeof(*s)); }
void settingsync_local_change(SettingSync *s) { if(s){ s->local_rev++; s->local_dirty=1; } }
void settingsync_observe_remote(SettingSync *s, uint64_t remote_rev) { if(s) s->remote_rev=remote_rev; }
int settingsync_decide(const SettingSync *s) {
    if(!s) return SS_NOOP;
    int local_ahead = s->local_rev>s->base_rev;
    int remote_ahead = s->remote_rev>s->base_rev;
    if (local_ahead && remote_ahead) return SS_CONFLICT;
    if (local_ahead) return SS_PUSH;
    if (remote_ahead) return SS_PULL;
    return SS_NOOP;
}
int settingsync_resolve_push(SettingSync *s) {
    if(!s) return -1;
    s->base_rev=s->local_rev; s->remote_rev=s->local_rev; s->local_dirty=0; return 0;
}
int settingsync_resolve_pull(SettingSync *s) {
    if(!s) return -1;
    s->base_rev=s->remote_rev; s->local_rev=s->remote_rev; s->local_dirty=0; return 0;
}
