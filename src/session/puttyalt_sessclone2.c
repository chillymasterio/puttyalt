/* puttyalt_sessclone2.c - Session clone v2 with field selection. */
#include <string.h>
#include <stdio.h>
typedef struct { char name[64]; char host[128]; int port; char user[64]; char tags[64]; char notes[160]; } sc_session;
#define SC_HOST 1
#define SC_AUTH 2
#define SC_TAGS 4
#define SC_NOTES 8
#define SC_ALL 15
int sessclone2(const sc_session *src, sc_session *dst, const char *new_name, int fields) {
    if(!src||!dst) return -1;
    memset(dst,0,sizeof(*dst));
    snprintf(dst->name,64,"%s",new_name?new_name:src->name);
    if (fields&SC_HOST) { snprintf(dst->host,128,"%s",src->host); dst->port=src->port; }
    if (fields&SC_AUTH) snprintf(dst->user,64,"%s",src->user);
    if (fields&SC_TAGS) snprintf(dst->tags,64,"%s",src->tags);
    if (fields&SC_NOTES) snprintf(dst->notes,160,"%s",src->notes);
    return 0;
}
int sessclone2_diff(const sc_session *a, const sc_session *b) {
    if(!a||!b) return -1;
    int diff=0;
    if (strcmp(a->host,b->host)!=0||a->port!=b->port) diff|=SC_HOST;
    if (strcmp(a->user,b->user)!=0) diff|=SC_AUTH;
    if (strcmp(a->tags,b->tags)!=0) diff|=SC_TAGS;
    if (strcmp(a->notes,b->notes)!=0) diff|=SC_NOTES;
    return diff;
}
