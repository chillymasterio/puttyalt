/* puttyalt_wasmhost.c - WASM host-call dispatch table with capability checks. */
#include <string.h>
#include <stdio.h>
#define WH_MAX 32
#define WH_NAME 40
typedef struct { char name[WH_NAME]; int cap_required; int call_count; } wh_func;
typedef struct { wh_func f[WH_MAX]; int n; int granted_caps; } WasmHost;
void wasmhost_init(WasmHost *h, int granted_caps) { if(h){ memset(h,0,sizeof(*h)); h->granted_caps=granted_caps; } }
int wasmhost_register(WasmHost *h, const char *name, int cap_required) {
    if(!h||h->n>=WH_MAX||!name) return -1;
    snprintf(h->f[h->n].name,WH_NAME,"%s",name); h->f[h->n].cap_required=cap_required; return h->n++;
}
int wasmhost_call(WasmHost *h, const char *name) {
    if(!h||!name) return -1;
    for (int i=0;i<h->n;i++) if (strcmp(h->f[i].name,name)==0) {
        if ((h->f[i].cap_required & ~h->granted_caps) != 0) return -2; /* permission denied */
        h->f[i].call_count++; return 0;
    }
    return -1; /* unknown host function */
}
int wasmhost_call_count(const WasmHost *h, const char *name) {
    if(!h||!name) return -1;
    for (int i=0;i<h->n;i++) if (strcmp(h->f[i].name,name)==0) return h->f[i].call_count;
    return -1;
}
int wasmhost_func_count(const WasmHost *h) { return h?h->n:-1; }
