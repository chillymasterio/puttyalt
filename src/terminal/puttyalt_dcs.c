/* puttyalt_dcs.c - DCS (Device Control String) parser + dispatcher. */
#include <string.h>
#include <stdio.h>
#define DCS_MAX 1024
enum dcs_state { DCS_IDLE=0, DCS_PARAMS=1, DCS_PASSTHROUGH=2 };
typedef struct { int state; char params[64]; char data[DCS_MAX]; int data_len; char final; } Dcs;
void dcs_init(Dcs *d) { if(d) memset(d,0,sizeof(*d)); }
int dcs_start(Dcs *d, const char *params, char final) {
    if(!d) return -1;
    d->state=DCS_PASSTHROUGH; snprintf(d->params,64,"%s",params?params:""); d->final=final; d->data_len=0;
    return 0;
}
int dcs_feed(Dcs *d, const char *data, int len) {
    if(!d||d->state!=DCS_PASSTHROUGH) return -1;
    int avail=DCS_MAX-1-d->data_len; if(len>avail)len=avail;
    memcpy(d->data+d->data_len,data,len); d->data_len+=len; d->data[d->data_len]=0;
    return len;
}
int dcs_terminate(Dcs *d) {
    if(!d||d->state!=DCS_PASSTHROUGH) return -1;
    d->state=DCS_IDLE; return d->data_len;
}
/* classify DCS by final char: q=Sixel, p=DECRSTS, |=DECUDK, $q=DECRQSS */
int dcs_classify(const Dcs *d) {
    if(!d) return -1;
    switch(d->final) { case 'q': return 1; /* sixel */ case '|': return 2; /* udk */ case 'p': return 3; default: return 0; }
}
const char *dcs_data(const Dcs *d) { return d?d->data:""; }
