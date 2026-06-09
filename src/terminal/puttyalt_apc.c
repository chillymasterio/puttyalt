/* puttyalt_apc.c - APC (Application Program Command) string handler. */
#include <string.h>
#include <stdio.h>
#define APC_MAX 4096
typedef struct { char data[APC_MAX]; int len; int active; } Apc;
void apc_init(Apc *a) { if(a) memset(a,0,sizeof(*a)); }
void apc_start(Apc *a) { if(a){ a->active=1; a->len=0; a->data[0]=0; } }
int apc_feed(Apc *a, const char *data, int len) {
    if(!a||!a->active) return -1;
    int avail=APC_MAX-1-a->len; if(len>avail)len=avail;
    memcpy(a->data+a->len,data,len); a->len+=len; a->data[a->len]=0; return len;
}
int apc_terminate(Apc *a) { if(!a) return -1; a->active=0; return a->len; }
/* Kitty graphics APC starts with 'G' */
int apc_is_kitty(const Apc *a) { return (a&&a->len>0&&a->data[0]=='G')?1:0; }
const char *apc_data(const Apc *a) { return a?a->data:""; }
int apc_length(const Apc *a) { return a?a->len:-1; }
