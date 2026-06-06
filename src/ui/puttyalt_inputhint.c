/* puttyalt_inputhint.c - Inline input hints/ghost text from history prediction. */
#include <string.h>
#include <stdio.h>
#define IH_MAX 64
#define IH_CMD 128
typedef struct { char cmd[IH_CMD]; int freq; } ih_entry;
typedef struct { ih_entry e[IH_MAX]; int n; } InputHint;
void inputhint_init(InputHint *h) { if(h) memset(h,0,sizeof(*h)); }
int inputhint_learn(InputHint *h, const char *cmd) {
    if(!h||!cmd||!*cmd) return -1;
    for (int i=0;i<h->n;i++) if (strcmp(h->e[i].cmd,cmd)==0) { h->e[i].freq++; return 0; }
    if (h->n>=IH_MAX) { /* evict least frequent */ int m=0; for(int i=1;i<h->n;i++) if(h->e[i].freq<h->e[m].freq)m=i; h->e[m]=h->e[h->n-1]; h->n--; }
    snprintf(h->e[h->n].cmd,IH_CMD,"%s",cmd); h->e[h->n].freq=1; h->n++; return 0;
}
int inputhint_predict(const InputHint *h, const char *prefix, char *out, int outlen) {
    if(!h||!prefix||!out) return -1;
    int pl=(int)strlen(prefix); if(pl==0) return -1;
    int best=-1;
    for (int i=0;i<h->n;i++) if (strncmp(h->e[i].cmd,prefix,pl)==0 && strlen(h->e[i].cmd)>(size_t)pl) {
        if (best<0||h->e[i].freq>h->e[best].freq) best=i;
    }
    if (best<0) return -1;
    snprintf(out,outlen,"%s",h->e[best].cmd+pl); /* ghost suffix only */
    return 0;
}
int inputhint_count(const InputHint *h) { return h?h->n:-1; }
