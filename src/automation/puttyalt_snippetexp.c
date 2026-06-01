/* puttyalt_snippetexp.c - Snippet placeholder expansion (${var}, ${cursor}, ${date}). */
#include <string.h>
#include <stdio.h>
#define SE_MAX_VARS 32
#define SE_NAME 32
#define SE_VAL 128
typedef struct { char name[SE_NAME]; char val[SE_VAL]; } se_var;
typedef struct { se_var v[SE_MAX_VARS]; int n; int cursor_pos; } SnippetExp;
void snippetexp_init(SnippetExp *s) { if(s) { memset(s,0,sizeof(*s)); s->cursor_pos=-1; } }
int snippetexp_set_var(SnippetExp *s, const char *name, const char *val) {
    if(!s||!name) return -1;
    for (int i=0;i<s->n;i++) if (strcmp(s->v[i].name,name)==0) { snprintf(s->v[i].val,SE_VAL,"%s",val?val:""); return 0; }
    if (s->n>=SE_MAX_VARS) return -1;
    snprintf(s->v[s->n].name,SE_NAME,"%s",name); snprintf(s->v[s->n].val,SE_VAL,"%s",val?val:""); s->n++; return 0;
}
static const char *se_lookup(const SnippetExp *s, const char *name, int len) {
    for (int i=0;i<s->n;i++) if ((int)strlen(s->v[i].name)==len && strncmp(s->v[i].name,name,len)==0) return s->v[i].val;
    return NULL;
}
int snippetexp_expand(SnippetExp *s, const char *tmpl, char *out, int outlen) {
    if(!s||!tmpl||!out||outlen<=0) return -1;
    int o=0; s->cursor_pos=-1;
    for (const char *p=tmpl; *p && o<outlen-1; ) {
        if (p[0]=='$' && p[1]=='{') {
            const char *end=strchr(p+2,'}');
            if (end) {
                int len=(int)(end-(p+2));
                if (len==6 && strncmp(p+2,"cursor",6)==0) { s->cursor_pos=o; }
                else {
                    const char *val=se_lookup(s,p+2,len);
                    if (val) o+=snprintf(out+o,outlen-o,"%s",val);
                }
                p=end+1; continue;
            }
        }
        out[o++]=*p++;
    }
    out[o]=0; return o;
}
int snippetexp_cursor(const SnippetExp *s) { return s?s->cursor_pos:-1; }
