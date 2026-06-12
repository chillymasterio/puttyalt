/* puttyalt_cmdparse.c - Command line tokenizer (quotes, escapes, pipes). */
#include <string.h>
#define CP_MAX_TOK 32
#define CP_TOK 96
typedef struct { char tokens[CP_MAX_TOK][CP_TOK]; int n; int has_pipe; int has_redirect; } CmdParse;
void cmdparse_init(CmdParse *c) { if(c) memset(c,0,sizeof(*c)); }
int cmdparse_tokenize(CmdParse *c, const char *cmd) {
    if(!c||!cmd) return -1;
    c->n=0; c->has_pipe=0; c->has_redirect=0;
    const char *p=cmd; int ti=0; int in_quote=0; char qc=0;
    while (*p && c->n<CP_MAX_TOK) {
        char ch=*p;
        if (in_quote) {
            if (ch==qc) in_quote=0;
            else if (ti<CP_TOK-1) c->tokens[c->n][ti++]=ch;
        } else if (ch=='"'||ch=='\'') { in_quote=1; qc=ch; }
        else if (ch=='\\' && p[1]) { p++; if(ti<CP_TOK-1) c->tokens[c->n][ti++]=*p; }
        else if (ch==' '||ch=='\t') { if(ti>0){ c->tokens[c->n][ti]=0; c->n++; ti=0; } }
        else if (ch=='|') { c->has_pipe=1; if(ti>0){c->tokens[c->n][ti]=0;c->n++;ti=0;} }
        else if (ch=='>'||ch=='<') { c->has_redirect=1; if(ti<CP_TOK-1) c->tokens[c->n][ti++]=ch; }
        else if (ti<CP_TOK-1) c->tokens[c->n][ti++]=ch;
        p++;
    }
    if (ti>0) { c->tokens[c->n][ti]=0; c->n++; }
    return c->n;
}
const char *cmdparse_token(const CmdParse *c, int idx) {
    return (c&&idx>=0&&idx<c->n)?c->tokens[idx]:0;
}
int cmdparse_count(const CmdParse *c) { return c?c->n:-1; }
