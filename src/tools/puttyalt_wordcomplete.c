/* puttyalt_wordcomplete.c - Word completion from terminal buffer vocabulary. */
#include <string.h>
#include <stdio.h>
#define WC_MAX 256
#define WC_WORD 32
typedef struct { char word[WC_WORD]; int freq; } wc_word;
typedef struct { wc_word words[WC_MAX]; int n; } WordComplete;
void wordcomplete_init(WordComplete *w) { if(w) memset(w,0,sizeof(*w)); }
int wordcomplete_learn(WordComplete *w, const char *word) {
    if(!w||!word||strlen(word)<3) return -1;
    for (int i=0;i<w->n;i++) if (strcmp(w->words[i].word,word)==0) { w->words[i].freq++; return 0; }
    if (w->n>=WC_MAX) { int m=0; for(int i=1;i<w->n;i++) if(w->words[i].freq<w->words[m].freq)m=i; w->words[m]=w->words[w->n-1]; w->n--; }
    snprintf(w->words[w->n].word,WC_WORD,"%s",word); w->words[w->n].freq=1; w->n++; return 0;
}
int wordcomplete_learn_line(WordComplete *w, const char *line) {
    if(!w||!line) return -1; int n=0; char word[WC_WORD]; int wl=0;
    for (const char *p=line; ;p++) {
        char c=*p;
        if ((c>='a'&&c<='z')||(c>='A'&&c<='Z')||(c>='0'&&c<='9')||c=='_'||c=='-') { if(wl<WC_WORD-1) word[wl++]=c; }
        else { if (wl>=3) { word[wl]=0; wordcomplete_learn(w,word); n++; } wl=0; }
        if (!c) break;
    }
    return n;
}
int wordcomplete_suggest(const WordComplete *w, const char *prefix, char *out, int outlen) {
    if(!w||!prefix) return -1; int pl=(int)strlen(prefix); if(pl==0) return -1;
    int best=-1;
    for (int i=0;i<w->n;i++) if (strncmp(w->words[i].word,prefix,pl)==0 && (int)strlen(w->words[i].word)>pl) {
        if (best<0||w->words[i].freq>w->words[best].freq) best=i;
    }
    if (best<0) return -1;
    snprintf(out,outlen,"%s",w->words[best].word); return 0;
}
int wordcomplete_count(const WordComplete *w) { return w?w->n:-1; }
