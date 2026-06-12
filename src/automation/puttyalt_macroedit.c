/* puttyalt_macroedit.c - Macro editor (insert/delete/reorder steps). */
#include <string.h>
#include <stdio.h>
#define ME_MAX 64
#define ME_DATA 96
typedef struct { char data[ME_DATA]; int delay_ms; int type; } me_step;
typedef struct { me_step steps[ME_MAX]; int n; char name[48]; } MacroEdit;
void macroedit_init(MacroEdit *m, const char *name) {
    if(!m) return;
    memset(m,0,sizeof(*m)); snprintf(m->name,48,"%s",name?name:"");
}
int macroedit_insert(MacroEdit *m, int pos, const char *data, int delay_ms, int type) {
    if(!m||m->n>=ME_MAX||pos<0||pos>m->n) return -1;
    memmove(&m->steps[pos+1],&m->steps[pos],sizeof(me_step)*(m->n-pos));
    me_step *s=&m->steps[pos]; snprintf(s->data,ME_DATA,"%s",data?data:""); s->delay_ms=delay_ms; s->type=type;
    m->n++; return pos;
}
int macroedit_delete(MacroEdit *m, int pos) {
    if(!m||pos<0||pos>=m->n) return -1;
    memmove(&m->steps[pos],&m->steps[pos+1],sizeof(me_step)*(m->n-pos-1)); m->n--; return 0;
}
int macroedit_move(MacroEdit *m, int from, int to) {
    if(!m||from<0||from>=m->n||to<0||to>=m->n) return -1;
    me_step tmp=m->steps[from];
    if (from<to) memmove(&m->steps[from],&m->steps[from+1],sizeof(me_step)*(to-from));
    else memmove(&m->steps[to+1],&m->steps[to],sizeof(me_step)*(from-to));
    m->steps[to]=tmp; return 0;
}
int macroedit_duration(const MacroEdit *m) {
    if(!m) return 0;
    int d=0; for(int i=0;i<m->n;i++) d+=m->steps[i].delay_ms; return d;
}
int macroedit_count(const MacroEdit *m) { return m?m->n:-1; }
