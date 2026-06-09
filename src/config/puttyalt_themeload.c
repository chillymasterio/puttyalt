/* puttyalt_themeload.c - Theme file loader (iTerm/Windows Terminal format). */
#include <string.h>
#include <stdio.h>
#include <stdint.h>
typedef struct { uint32_t colors[16]; uint32_t fg, bg; int loaded; } ThemeLoad;
static uint32_t tl_parse_color(const char *s) {
    uint32_t c=0;
    if (s[0]=='#') sscanf(s+1,"%6x",&c);
    else if (!strncmp(s,"0x",2)) sscanf(s+2,"%x",&c);
    else sscanf(s,"%x",&c);
    return c;
}
void themeload_init(ThemeLoad *t) { if(t) memset(t,0,sizeof(*t)); }
int themeload_parse_json(ThemeLoad *t, const char *json) {
    if(!t||!json) return -1;
    char key[16];
    for (int i=0;i<16;i++) {
        snprintf(key,sizeof(key),"\"color%d\"",i);
        const char *p=strstr(json,key);
        if (p) { p=strchr(p,':'); if(p){ p=strchr(p,'#'); if(p) t->colors[i]=tl_parse_color(p); } }
    }
    const char *fg=strstr(json,"\"foreground\""); if(fg){ fg=strchr(fg,'#'); if(fg) t->fg=tl_parse_color(fg); }
    const char *bg=strstr(json,"\"background\""); if(bg){ bg=strchr(bg,'#'); if(bg) t->bg=tl_parse_color(bg); }
    t->loaded=1; return 0;
}
uint32_t themeload_color(const ThemeLoad *t, int idx) {
    return (t&&idx>=0&&idx<16)?t->colors[idx]:0;
}
int themeload_is_loaded(const ThemeLoad *t) { return t?t->loaded:-1; }
