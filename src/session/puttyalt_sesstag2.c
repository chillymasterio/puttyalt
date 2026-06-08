/* puttyalt_sesstag2.c - Session tagging v2 with colors and AND/OR filters. */
#include <string.h>
#include <stdio.h>
#define ST_MAX 48
#define ST_NAME 24
typedef struct { char name[ST_NAME]; unsigned int color; int count; } st_tag;
typedef struct { st_tag tags[ST_MAX]; int n; } SessTag2;
void sesstag2_init(SessTag2 *t) { if(t) memset(t,0,sizeof(*t)); }
int sesstag2_define(SessTag2 *t, const char *name, unsigned int color) {
    if(!t||!name) return -1;
    for (int i=0;i<t->n;i++) if (strcmp(t->tags[i].name,name)==0) { t->tags[i].color=color; return i; }
    if (t->n>=ST_MAX) return -1;
    snprintf(t->tags[t->n].name,ST_NAME,"%s",name); t->tags[t->n].color=color; return t->n++;
}
unsigned int sesstag2_color(const SessTag2 *t, const char *name) {
    if(!t||!name) return 0;
    for (int i=0;i<t->n;i++) if (strcmp(t->tags[i].name,name)==0) return t->tags[i].color;
    return 0;
}
/* csv_tags="a,b,c"; filter="a&b" (AND) or "a|c" (OR). */
int sesstag2_matches(const char *csv_tags, const char *filter) {
    if(!csv_tags||!filter) return 0;
    if (strchr(filter,'&')) {
        char f[128]; snprintf(f,sizeof(f),"%s",filter);
        char *p=f, *amp;
        do {
            amp=strchr(p,'&'); if(amp)*amp=0;
            if (!strstr(csv_tags,p)) return 0;
            p=amp?amp+1:0;
        } while (p);
        return 1;
    }
    if (strchr(filter,'|')) {
        char f[128]; snprintf(f,sizeof(f),"%s",filter);
        char *p=f, *pipe;
        do {
            pipe=strchr(p,'|'); if(pipe)*pipe=0;
            if (strstr(csv_tags,p)) return 1;
            p=pipe?pipe+1:0;
        } while (p);
        return 0;
    }
    return strstr(csv_tags,filter)?1:0;
}
int sesstag2_count(const SessTag2 *t) { return t?t->n:-1; }
