/* puttyalt_breadtrail.c - Navigation breadcrumb trail for nested session folders. */
#include <string.h>
#include <stdio.h>
#define BT_MAX 16
#define BT_SEG 48
typedef struct { char seg[BT_MAX][BT_SEG]; int depth; } BreadTrail;
void breadtrail_init(BreadTrail *b) { if(b) memset(b,0,sizeof(*b)); }
int breadtrail_push(BreadTrail *b, const char *seg) {
    if(!b||b->depth>=BT_MAX||!seg) return -1;
    snprintf(b->seg[b->depth++],BT_SEG,"%s",seg); return 0;
}
int breadtrail_pop(BreadTrail *b) { if(!b||b->depth==0) return -1; b->depth--; return 0; }
int breadtrail_to(BreadTrail *b, int depth) { if(!b||depth<0||depth>b->depth) return -1; b->depth=depth; return 0; }
int breadtrail_render(const BreadTrail *b, char *buf, int buflen) {
    if(!b||!buf) return -1;
    int pos=0;
    for (int i=0;i<b->depth && pos<buflen;i++) pos+=snprintf(buf+pos,buflen-pos,"%s%s",i?" > ":"",b->seg[i]);
    return pos;
}
int breadtrail_depth(const BreadTrail *b) { return b?b->depth:-1; }
