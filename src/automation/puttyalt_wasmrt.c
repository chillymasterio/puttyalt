/* puttyalt_wasmrt.c - WASM plugin runtime descriptor: module load + import gating. */
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#define WR_MAX 16
#define WR_NAME 48
enum wr_state { WR_UNLOADED=0, WR_LOADED, WR_INSTANTIATED, WR_RUNNING, WR_TRAPPED };
typedef struct { char name[WR_NAME]; int state; uint32_t mem_pages; uint32_t fuel; int import_mask; } wr_module;
typedef struct { wr_module m[WR_MAX]; int n; uint32_t max_pages; uint32_t fuel_budget; } WasmRt;
void wasmrt_init(WasmRt *w, uint32_t max_pages, uint32_t fuel_budget) {
    if(!w) return; memset(w,0,sizeof(*w)); w->max_pages=max_pages?max_pages:256; w->fuel_budget=fuel_budget?fuel_budget:1000000;
}
int wasmrt_load(WasmRt *w, const char *name, uint32_t mem_pages, int import_mask) {
    if(!w||w->n>=WR_MAX||!name) return -1;
    if (mem_pages>w->max_pages) return -1;
    wr_module *m=&w->m[w->n]; snprintf(m->name,WR_NAME,"%s",name); m->state=WR_LOADED;
    m->mem_pages=mem_pages; m->fuel=w->fuel_budget; m->import_mask=import_mask; return w->n++;
}
int wasmrt_instantiate(WasmRt *w, int idx, int granted_imports) {
    if(!w||idx<0||idx>=w->n||w->m[idx].state!=WR_LOADED) return -1;
    if ((w->m[idx].import_mask & ~granted_imports) != 0) return -1; /* requests ungranted import */
    w->m[idx].state=WR_INSTANTIATED; return 0;
}
int wasmrt_consume_fuel(WasmRt *w, int idx, uint32_t amount) {
    if(!w||idx<0||idx>=w->n) return -1;
    if (amount>w->m[idx].fuel) { w->m[idx].fuel=0; w->m[idx].state=WR_TRAPPED; return -1; }
    w->m[idx].fuel-=amount; return 0;
}
int wasmrt_state(const WasmRt *w, int idx) { return (w&&idx>=0&&idx<w->n)?w->m[idx].state:-1; }
int wasmrt_module_count(const WasmRt *w) { return w?w->n:-1; }
