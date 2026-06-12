/* puttyalt_cloudprofile.c - Cloud profile sync metadata (revision, device, dirty set). */
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#define CP_MAX_DIRTY 32
#define CP_NAME 64
typedef struct { char device_id[CP_NAME]; uint64_t revision; uint64_t synced_rev; char dirty[CP_MAX_DIRTY][CP_NAME]; int ndirty; int enabled; } CloudProfile;
void cloudprofile_init(CloudProfile *p, const char *device_id) {
    if(!p) return;
    memset(p,0,sizeof(*p)); snprintf(p->device_id,CP_NAME,"%s",device_id?device_id:"");
}
void cloudprofile_set_enabled(CloudProfile *p, int on) { if(p) p->enabled=on?1:0; }
int cloudprofile_mark_dirty(CloudProfile *p, const char *key) {
    if(!p||!key) return -1;
    for (int i=0;i<p->ndirty;i++) if (strcmp(p->dirty[i],key)==0) return 0;
    if (p->ndirty>=CP_MAX_DIRTY) return -1;
    snprintf(p->dirty[p->ndirty++],CP_NAME,"%s",key); p->revision++; return 0;
}
int cloudprofile_needs_push(const CloudProfile *p) { return (p && p->enabled && p->revision>p->synced_rev)?1:0; }
int cloudprofile_mark_synced(CloudProfile *p, uint64_t remote_rev) {
    if(!p) return -1;
    p->synced_rev=remote_rev>p->revision?remote_rev:p->revision; p->revision=p->synced_rev; p->ndirty=0; return 0;
}
int cloudprofile_dirty_count(const CloudProfile *p) { return p?p->ndirty:-1; }
