/* puttyalt_aiprompt.c - Local AI prompt context builder (recent cmds + cwd + errors). */
#include <string.h>
#include <stdio.h>
#define AP_MAX_CTX 16
#define AP_LINE 256
typedef struct { char lines[AP_MAX_CTX][AP_LINE]; int n; char cwd[256]; char last_error[256]; int max_tokens; } AiPrompt;
void aiprompt_init(AiPrompt *a, int max_tokens) {
    if(!a) return;
    memset(a,0,sizeof(*a)); a->max_tokens=max_tokens>0?max_tokens:2048;
}
void aiprompt_set_cwd(AiPrompt *a, const char *cwd) { if(a) snprintf(a->cwd,256,"%s",cwd?cwd:""); }
void aiprompt_set_error(AiPrompt *a, const char *err) { if(a) snprintf(a->last_error,256,"%s",err?err:""); }
int aiprompt_add_context(AiPrompt *a, const char *line) {
    if(!a||!line) return -1;
    if (a->n>=AP_MAX_CTX) { memmove(a->lines[0],a->lines[1],sizeof(a->lines[0])*(AP_MAX_CTX-1)); a->n--; }
    snprintf(a->lines[a->n++],AP_LINE,"%s",line); return 0;
}
int aiprompt_build(const AiPrompt *a, const char *query, char *out, int outlen) {
    if(!a||!out) return -1;
    int pos=0;
    pos+=snprintf(out+pos,outlen-pos,"# context\ncwd: %s\n",a->cwd);
    if (a->last_error[0]) pos+=snprintf(out+pos,outlen-pos,"last_error: %s\n",a->last_error);
    pos+=snprintf(out+pos,outlen-pos,"recent:\n");
    for (int i=0;i<a->n && pos<outlen;i++) pos+=snprintf(out+pos,outlen-pos,"  %s\n",a->lines[i]);
    if (query) pos+=snprintf(out+pos,outlen-pos,"# query\n%s\n",query);
    return pos;
}
int aiprompt_context_lines(const AiPrompt *a) { return a?a->n:-1; }
