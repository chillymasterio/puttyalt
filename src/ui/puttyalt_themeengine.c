/* puttyalt_themeengine.c - Theme engine v3: live theme tokens + derived colors. */
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#define TE_MAX_TOKENS 48
#define TE_NAME 32
typedef struct { char name[TE_NAME]; uint32_t color; } te_token;
typedef struct { te_token tokens[TE_MAX_TOKENS]; int n; int dark_mode; } ThemeEngine;
void themeengine_init(ThemeEngine *t, int dark) { if(t){ memset(t,0,sizeof(*t)); t->dark_mode=dark?1:0; } }
int themeengine_set(ThemeEngine *t, const char *name, uint32_t color) {
    if(!t||!name) return -1;
    for (int i=0;i<t->n;i++) if (strcmp(t->tokens[i].name,name)==0) { t->tokens[i].color=color; return 0; }
    if (t->n>=TE_MAX_TOKENS) return -1;
    snprintf(t->tokens[t->n].name,TE_NAME,"%s",name); t->tokens[t->n].color=color; t->n++; return 0;
}
uint32_t themeengine_get(const ThemeEngine *t, const char *name, uint32_t fallback) {
    if(!t||!name) return fallback;
    for (int i=0;i<t->n;i++) if (strcmp(t->tokens[i].name,name)==0) return t->tokens[i].color;
    return fallback;
}
uint32_t themeengine_derive_hover(uint32_t base, int dark) {
    int r=(base>>16)&0xFF, g=(base>>8)&0xFF, b=base&0xFF;
    int d = dark? 24 : -24;
    r+=d; g+=d; b+=d;
    if(r<0)r=0; if(r>255)r=255; if(g<0)g=0; if(g>255)g=255; if(b<0)b=0; if(b>255)b=255;
    return (uint32_t)((r<<16)|(g<<8)|b);
}
uint32_t themeengine_mix(uint32_t a, uint32_t b, int t_x100) {
    if(t_x100<0)t_x100=0; if(t_x100>100)t_x100=100;
    int ra=(a>>16)&0xFF, ga=(a>>8)&0xFF, ba=a&0xFF;
    int rb=(b>>16)&0xFF, gb=(b>>8)&0xFF, bb=b&0xFF;
    int r=(ra*(100-t_x100)+rb*t_x100)/100;
    int g=(ga*(100-t_x100)+gb*t_x100)/100;
    int bl=(ba*(100-t_x100)+bb*t_x100)/100;
    return (uint32_t)((r<<16)|(g<<8)|bl);
}
int themeengine_token_count(const ThemeEngine *t) { return t?t->n:-1; }
