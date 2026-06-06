/* puttyalt_keymap2.c - Keymap v2 with chord sequences + context layers. */
#include <string.h>
#include <stdio.h>
#define KM_MAX 96
#define KM_CHORD 24
typedef struct { char chord[KM_CHORD]; int action_id; int context; } km_binding;
typedef struct { km_binding b[KM_MAX]; int n; int active_context; char pending[KM_CHORD]; } Keymap2;
void keymap2_init(Keymap2 *k) { if(k){ memset(k,0,sizeof(*k)); } }
int keymap2_bind(Keymap2 *k, const char *chord, int action_id, int context) {
    if(!k||k->n>=KM_MAX||!chord) return -1;
    km_binding *b=&k->b[k->n++]; snprintf(b->chord,KM_CHORD,"%s",chord); b->action_id=action_id; b->context=context;
    return 0;
}
void keymap2_set_context(Keymap2 *k, int context) { if(k) k->active_context=context; }
int keymap2_feed(Keymap2 *k, const char *key) {
    if(!k||!key) return -1;
    /* append to pending chord */
    int pl=(int)strlen(k->pending);
    snprintf(k->pending+pl,KM_CHORD-pl," %s",key);
    if (k->pending[0]==' ') memmove(k->pending,k->pending+1,strlen(k->pending));
    /* exact match? */
    for (int i=0;i<k->n;i++) if (k->b[i].context==k->active_context && strcmp(k->b[i].chord,k->pending)==0) {
        int a=k->b[i].action_id; k->pending[0]=0; return a;
    }
    /* prefix of any binding? keep waiting */
    for (int i=0;i<k->n;i++) if (k->b[i].context==k->active_context && strncmp(k->b[i].chord,k->pending,strlen(k->pending))==0) return -2; /* partial */
    k->pending[0]=0; return -1; /* no match, reset */
}
void keymap2_reset_pending(Keymap2 *k) { if(k) k->pending[0]=0; }
int keymap2_count(const Keymap2 *k) { return k?k->n:-1; }
