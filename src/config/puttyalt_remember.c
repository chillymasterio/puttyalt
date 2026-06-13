/* puttyalt_remember.c - Remember last-used settings per host. */
#include <string.h>
#include <stdio.h>
#define RM_MAX 64
#define RM_HOST 96
typedef struct { char host[RM_HOST]; int port; char user[48]; int last_window_w, last_window_h; int color_scheme; } rm_entry;
typedef struct { rm_entry e[RM_MAX]; int n; } Remember;
void remember_init(Remember *r) { if(r) memset(r,0,sizeof(*r)); }
static rm_entry *rm_find(Remember *r, const char *host) {
    for(int i=0;i<r->n;i++) if(strcmp(r->e[i].host,host)==0) return &r->e[i];
    if (r->n>=RM_MAX) return 0;
    rm_entry *e=&r->e[r->n++]; snprintf(e->host,RM_HOST,"%s",host); return e;
}
int remember_save(Remember *r, const char *host, int port, const char *user, int scheme) {
    if(!r||!host) return -1;
    rm_entry *e=rm_find(r,host); if(!e) return -1;
    e->port=port; snprintf(e->user,48,"%s",user?user:""); e->color_scheme=scheme; return 0;
}
int remember_window(Remember *r, const char *host, int w, int h) {
    if(!r||!host) return -1;
    rm_entry *e=rm_find(r,host); if(!e) return -1;
    e->last_window_w=w; e->last_window_h=h; return 0;
}
int remember_recall(const Remember *r, const char *host, int *port, char *user, int userlen, int *scheme) {
    if(!r||!host) return -1;
    for (int i=0;i<r->n;i++) if (strcmp(r->e[i].host,host)==0) {
        if(port) *port=r->e[i].port;
        if(user)snprintf(user,userlen,"%s",r->e[i].user); if(scheme)*scheme=r->e[i].color_scheme;
        return 0;
    }
    return -1;
}
int remember_count(const Remember *r) { return r?r->n:-1; }
