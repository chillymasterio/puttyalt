/* puttyalt_undostack.c - Undo/redo stack for editor operations. */
#include <string.h>
#include <stdio.h>
#define US_MAX 128
#define US_DATA 64
enum us_op { US_INSERT=0, US_DELETE=1, US_REPLACE=2 };
typedef struct { int op; int pos; char text[US_DATA]; char prev[US_DATA]; } us_entry;
typedef struct { us_entry entries[US_MAX]; int n; int cursor; } UndoStack;
void undostack_init(UndoStack *u) { if(u) memset(u,0,sizeof(*u)); }
int undostack_push(UndoStack *u, int op, int pos, const char *text, const char *prev) {
    if(!u) return -1;
    /* truncate redo history */
    u->n=u->cursor;
    if (u->n>=US_MAX) { memmove(&u->entries[0],&u->entries[1],sizeof(us_entry)*(US_MAX-1)); u->n--; }
    us_entry *e=&u->entries[u->n++]; e->op=op; e->pos=pos;
    snprintf(e->text,US_DATA,"%s",text?text:""); snprintf(e->prev,US_DATA,"%s",prev?prev:"");
    u->cursor=u->n; return 0;
}
int undostack_can_undo(const UndoStack *u) { return (u&&u->cursor>0)?1:0; }
int undostack_can_redo(const UndoStack *u) { return (u&&u->cursor<u->n)?1:0; }
int undostack_undo(UndoStack *u, int *op, int *pos, char *text, int textlen) {
    if(!u||u->cursor==0) return -1;
    u->cursor--; us_entry *e=&u->entries[u->cursor];
    if(op)*op=e->op; if(pos)*pos=e->pos; if(text)snprintf(text,textlen,"%s",e->prev); return 0;
}
int undostack_redo(UndoStack *u, int *op, int *pos, char *text, int textlen) {
    if(!u||u->cursor>=u->n) return -1;
    us_entry *e=&u->entries[u->cursor]; u->cursor++;
    if(op)*op=e->op; if(pos)*pos=e->pos; if(text)snprintf(text,textlen,"%s",e->text); return 0;
}
int undostack_count(const UndoStack *u) { return u?u->n:-1; }
