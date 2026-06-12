/* puttyalt_authmethod.c - SSH auth method negotiation order + attempt tracking. */
#include <string.h>
#include <stdio.h>
#define AM_MAX 8
enum am_method { AM_NONE=0, AM_PUBKEY, AM_PASSWORD, AM_KBDINT, AM_GSSAPI, AM_HOSTBASED };
typedef struct { int method; int tried; int succeeded; } am_entry;
typedef struct { am_entry m[AM_MAX]; int n; int current; } AuthMethod;
void authmethod_init(AuthMethod *a) { if(a){ memset(a,0,sizeof(*a)); a->current=-1; } }
int authmethod_offer(AuthMethod *a, int method) {
    if(!a||a->n>=AM_MAX) return -1;
    for (int i=0;i<a->n;i++) if (a->m[i].method==method) return 0;
    a->m[a->n++].method=method; return 0;
}
int authmethod_next(AuthMethod *a) {
    if(!a) return AM_NONE;
    for (int i=0;i<a->n;i++) if (!a->m[i].tried) { a->m[i].tried=1; a->current=i; return a->m[i].method; }
    return AM_NONE;
}
int authmethod_mark_result(AuthMethod *a, int success) {
    if(!a||a->current<0) return -1;
    a->m[a->current].succeeded=success?1:0; return 0;
}
int authmethod_succeeded(const AuthMethod *a) {
    if(!a) return -1;
    for(int i=0;i<a->n;i++) if(a->m[i].succeeded) return a->m[i].method; return AM_NONE;
}
int authmethod_count(const AuthMethod *a) { return a?a->n:-1; }
