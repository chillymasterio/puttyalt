/* puttyalt_luaapi.c - Lua scripting API binding registry (function table). */
#include <string.h>
#include <stdio.h>
#define LA_MAX 64
#define LA_NAME 40
typedef struct { char name[LA_NAME]; int arg_count; int returns; int call_count; } la_func;
typedef struct { la_func funcs[LA_MAX]; int n; } LuaApi;
void luaapi_init(LuaApi *l) { if(l) memset(l,0,sizeof(*l)); }
int luaapi_register(LuaApi *l, const char *name, int arg_count, int returns) {
    if(!l||l->n>=LA_MAX||!name) return -1;
    la_func *f=&l->funcs[l->n++]; snprintf(f->name,LA_NAME,"%s",name); f->arg_count=arg_count; f->returns=returns;
    return 0;
}
int luaapi_find(const LuaApi *l, const char *name) {
    if(!l||!name) return -1;
    for (int i=0;i<l->n;i++) if (strcmp(l->funcs[i].name,name)==0) return i;
    return -1;
}
int luaapi_call(LuaApi *l, const char *name, int provided_args) {
    if(!l||!name) return -1;
    int idx=luaapi_find(l,name); if(idx<0) return -1; /* unknown function */
    if (provided_args != l->funcs[idx].arg_count) return -2;
    /* arg count mismatch */
    l->funcs[idx].call_count++;
    return l->funcs[idx].returns;
}
int luaapi_count(const LuaApi *l) { return l?l->n:-1; }
