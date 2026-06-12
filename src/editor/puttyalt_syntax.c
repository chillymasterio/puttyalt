/* puttyalt_syntax.c - Basic syntax highlighting tokenizer (keywords/strings/comments). */
#include <string.h>
enum sy_token { SY_PLAIN=0, SY_KEYWORD=1, SY_STRING=2, SY_COMMENT=3, SY_NUMBER=4, SY_OPERATOR=5 };
typedef struct { const char *keywords[32]; int nkeywords; char comment_start[4]; char string_chars[4]; } Syntax;
void syntax_init(Syntax *s) {
    if(!s) return;
    memset(s,0,sizeof(*s));
    strcpy(s->comment_start,"#"); strcpy(s->string_chars,"\"'");
}
int syntax_add_keyword(Syntax *s, const char *kw) {
    if(!s||s->nkeywords>=32||!kw) return -1;
    s->keywords[s->nkeywords++]=kw; return 0;
}
static int sy_is_keyword(const Syntax *s, const char *word, int len) {
    for (int i=0;i<s->nkeywords;i++) if ((int)strlen(s->keywords[i])==len && strncmp(s->keywords[i],word,len)==0) return 1;
    return 0;
}
/* classify token starting at pos; returns token type and advances *end */
int syntax_token_at(const Syntax *s, const char *line, int pos, int *end) {
    if(!s||!line) return SY_PLAIN;
    char c=line[pos];
    if (strchr(s->comment_start,c)) { int e=pos; while(line[e])e++; if(end)*end=e; return SY_COMMENT; }
    if (strchr(s->string_chars,c)) { int e=pos+1; while(line[e]&&line[e]!=c)e++; if(line[e])e++; if(end)*end=e; return SY_STRING; }
    if (c>='0'&&c<='9') { int e=pos; while((line[e]>='0'&&line[e]<='9')||line[e]=='.')e++; if(end)*end=e; return SY_NUMBER; }
    if ((c>='a'&&c<='z')||(c>='A'&&c<='Z')||c=='_') {
        int e=pos; while((line[e]>='a'&&line[e]<='z')||(line[e]>='A'&&line[e]<='Z')||(line[e]>='0'&&line[e]<='9')||line[e]=='_')e++;
        if(end)*end=e; return sy_is_keyword(s,line+pos,e-pos)?SY_KEYWORD:SY_PLAIN;
    }
    if (strchr("+-*/=<>!&|",c)) { if(end)*end=pos+1; return SY_OPERATOR; }
    if(end)*end=pos+1; return SY_PLAIN;
}
