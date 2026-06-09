/* puttyalt_restendpoint.c - REST API endpoint router (method+path -> handler). */
#include <string.h>
#include <stdio.h>
#define RE_MAX 32
#define RE_PATH 96
enum re_method { RE_GET=0, RE_POST=1, RE_PUT=2, RE_DELETE=3 };
typedef struct { int method; char path[RE_PATH]; int handler_id; int auth_required; int hits; } re_route;
typedef struct { re_route routes[RE_MAX]; int n; } RestEndpoint;
void restendpoint_init(RestEndpoint *r) { if(r) memset(r,0,sizeof(*r)); }
int restendpoint_register(RestEndpoint *r, int method, const char *path, int handler_id, int auth) {
    if(!r||r->n>=RE_MAX||!path) return -1;
    re_route *rt=&r->routes[r->n++]; rt->method=method; snprintf(rt->path,RE_PATH,"%s",path); rt->handler_id=handler_id; rt->auth_required=auth?1:0;
    return 0;
}
static int re_path_match(const char *pattern, const char *path) {
    /* support trailing :param wildcard segment */
    const char *pp=pattern, *up=path;
    while (*pp && *up) {
        if (*pp==':') { while(*pp&&*pp!='/')pp++; while(*up&&*up!='/')up++; }
        else { if(*pp!=*up) return 0; pp++; up++; }
    }
    return (*pp==0 && *up==0)?1:0;
}
int restendpoint_route(RestEndpoint *r, int method, const char *path, int *out_handler, int *out_auth) {
    if(!r||!path) return -1;
    for (int i=0;i<r->n;i++) if (r->routes[i].method==method && re_path_match(r->routes[i].path,path)) {
        r->routes[i].hits++;
        if(out_handler)*out_handler=r->routes[i].handler_id;
        if(out_auth)*out_auth=r->routes[i].auth_required;
        return 0;
    }
    return -1; /* 404 */
}
int restendpoint_count(const RestEndpoint *r) { return r?r->n:-1; }
