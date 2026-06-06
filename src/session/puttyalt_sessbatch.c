/* puttyalt_sessbatch.c - Batch session operations (bulk connect/tag/move). */
#include <string.h>
#include <stdio.h>
#define SB_MAX 64
typedef struct { int session_ids[SB_MAX]; int n; int last_op; int affected; } SessBatch;
enum sb_op { SB_NONE=0, SB_CONNECT=1, SB_TAG=2, SB_MOVE=3, SB_DELETE=4, SB_EXPORT=5 };
void sessbatch_init(SessBatch *b) { if(b) memset(b,0,sizeof(*b)); }
int sessbatch_select(SessBatch *b, int session_id) {
    if(!b||b->n>=SB_MAX) return -1;
    for (int i=0;i<b->n;i++) if (b->session_ids[i]==session_id) return 1; /* already */
    b->session_ids[b->n++]=session_id; return 0;
}
int sessbatch_deselect(SessBatch *b, int session_id) {
    if(!b) return -1;
    for (int i=0;i<b->n;i++) if (b->session_ids[i]==session_id) {
        memmove(&b->session_ids[i],&b->session_ids[i+1],sizeof(int)*(b->n-i-1)); b->n--; return 0;
    }
    return -1;
}
int sessbatch_apply(SessBatch *b, int op) {
    if(!b||b->n==0) return -1; b->last_op=op; b->affected=b->n; return b->n;
}
void sessbatch_clear(SessBatch *b) { if(b){ b->n=0; b->last_op=SB_NONE; } }
int sessbatch_selected_count(const SessBatch *b) { return b?b->n:-1; }
