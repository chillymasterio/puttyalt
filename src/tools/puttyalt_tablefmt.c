/* puttyalt_tablefmt.c - ASCII table formatter with column alignment. */
#include <string.h>
#include <stdio.h>
#define TF_COLS 8
typedef struct { int widths[TF_COLS]; int ncols; int aligns[TF_COLS]; } TableFmt;
void tablefmt_init(TableFmt *t, int ncols) {
    if(!t) return;
    memset(t,0,sizeof(*t)); t->ncols=ncols>TF_COLS?TF_COLS:ncols;
}
void tablefmt_measure(TableFmt *t, const char **cells) {
    if(!t||!cells) return;
    for (int i=0;i<t->ncols;i++) { int l=(int)strlen(cells[i]); if(l>t->widths[i]) t->widths[i]=l; }
}
void tablefmt_set_align(TableFmt *t, int col, int align) { if(t&&col>=0&&col<t->ncols) t->aligns[col]=align; }
int tablefmt_row(const TableFmt *t, const char **cells, char *out, int outlen) {
    if(!t||!cells||!out) return -1;
    int pos=0;
    for (int i=0;i<t->ncols && pos<outlen;i++) {
        int w=t->widths[i], l=(int)strlen(cells[i]);
        if (i>0) out[pos++]=' ';
        if (out[pos-1]||i==0) {} /* sep */
        if (i>0) { out[pos++]='|'; out[pos++]=' '; }
        if (t->aligns[i]==1) { for(int j=l;j<w&&pos<outlen;j++) out[pos++]=' '; }
        pos+=snprintf(out+pos,outlen-pos,"%s",cells[i]);
        if (t->aligns[i]!=1) { for(int j=l;j<w&&pos<outlen;j++) out[pos++]=' '; }
    }
    out[pos]=0; return pos;
}
int tablefmt_separator(const TableFmt *t, char *out, int outlen) {
    if(!t||!out) return -1;
    int pos=0;
    for (int i=0;i<t->ncols && pos<outlen;i++) {
        if (i>0) { out[pos++]='-'; out[pos++]='+'; out[pos++]='-'; }
        for (int j=0;j<t->widths[i]&&pos<outlen;j++) out[pos++]='-';
    }
    out[pos]=0; return pos;
}
