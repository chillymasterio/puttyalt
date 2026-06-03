/* puttyalt_notifcenter2.c - Notification center v2: grouped + actionable + history. */
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#define NC_MAX 64
#define NC_TEXT 160
enum nc_sev { NC_INFO=0, NC_SUCCESS=1, NC_WARN=2, NC_ERROR=3 };
typedef struct { char text[NC_TEXT]; int sev; int read; int has_action; uint64_t ts; char group[32]; } nc_item;
typedef struct { nc_item items[NC_MAX]; int n; int unread; } NotifCenter2;
void notifcenter2_init(NotifCenter2 *n) { if(n) memset(n,0,sizeof(*n)); }
int notifcenter2_push(NotifCenter2 *n, const char *text, int sev, const char *group, int has_action, uint64_t now_ms) {
    if(!n||!text) return -1;
    if (n->n>=NC_MAX) { if(!n->items[0].read) n->unread--; memmove(&n->items[0],&n->items[1],sizeof(nc_item)*(NC_MAX-1)); n->n--; }
    nc_item *it=&n->items[n->n++]; snprintf(it->text,NC_TEXT,"%s",text); it->sev=sev;
    snprintf(it->group,32,"%s",group?group:"general"); it->has_action=has_action?1:0; it->ts=now_ms; it->read=0; n->unread++;
    return 0;
}
int notifcenter2_mark_read(NotifCenter2 *n, int idx) {
    if(!n||idx<0||idx>=n->n) return -1;
    if (!n->items[idx].read) { n->items[idx].read=1; n->unread--; }
    return 0;
}
void notifcenter2_mark_all_read(NotifCenter2 *n) {
    if(!n) return; for(int i=0;i<n->n;i++) n->items[i].read=1; n->unread=0;
}
int notifcenter2_group_count(const NotifCenter2 *n, const char *group) {
    if(!n||!group) return -1; int c=0; for(int i=0;i<n->n;i++) if(strcmp(n->items[i].group,group)==0)c++; return c;
}
int notifcenter2_unread(const NotifCenter2 *n) { return n?n->unread:-1; }
