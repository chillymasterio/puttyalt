/* puttyalt_hexedit.c - Hex editor view model (byte buffer + cursor + edits). */
#include <string.h>
#include <stdio.h>
#define HE_MAX 4096
typedef struct { unsigned char data[HE_MAX]; int len; int cursor; int dirty; int bytes_per_row; } HexEdit;
void hexedit_init(HexEdit *h, int bytes_per_row) {
    if(!h) return; memset(h,0,sizeof(*h)); h->bytes_per_row=bytes_per_row>0?bytes_per_row:16;
}
int hexedit_load(HexEdit *h, const unsigned char *data, int len) {
    if(!h||!data) return -1; int cl=len<HE_MAX?len:HE_MAX;
    memcpy(h->data,data,cl); h->len=cl; h->cursor=0; h->dirty=0; return cl;
}
int hexedit_set_byte(HexEdit *h, int offset, unsigned char val) {
    if(!h||offset<0||offset>=h->len) return -1;
    h->data[offset]=val; h->dirty=1; return 0;
}
void hexedit_move_cursor(HexEdit *h, int delta) {
    if(!h) return; h->cursor+=delta; if(h->cursor<0)h->cursor=0; if(h->cursor>=h->len)h->cursor=h->len-1;
}
int hexedit_render_row(const HexEdit *h, int row, char *buf, int buflen) {
    if(!h||!buf) return -1;
    int start=row*h->bytes_per_row; if (start>=h->len) return 0;
    int pos=snprintf(buf,buflen,"%08X  ",start);
    for (int i=0;i<h->bytes_per_row && start+i<h->len;i++) pos+=snprintf(buf+pos,buflen-pos,"%02X ",h->data[start+i]);
    pos+=snprintf(buf+pos,buflen-pos," |");
    for (int i=0;i<h->bytes_per_row && start+i<h->len;i++) {
        unsigned char c=h->data[start+i]; pos+=snprintf(buf+pos,buflen-pos,"%c",(c>=32&&c<127)?c:'.');
    }
    pos+=snprintf(buf+pos,buflen-pos,"|");
    return pos;
}
int hexedit_row_count(const HexEdit *h) { return h? (h->len+h->bytes_per_row-1)/h->bytes_per_row:-1; }
