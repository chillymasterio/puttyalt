/* puttyalt_keymapui.c - Keyboard shortcut UI editor with conflict detection. */
#include <string.h>
#include <stdio.h>
#define KU_MAX 96
#define KU_KEY 24
#define KU_ACT 48
typedef struct { char keys[KU_KEY]; char action[KU_ACT]; int context; } ku_binding;
typedef struct { ku_binding b[KU_MAX]; int n; } KeymapUi;
void keymapui_init(KeymapUi *k) { if(k) memset(k,0,sizeof(*k)); }
int keymapui_bind(KeymapUi *k, const char *keys, const char *action, int context) {
    if(!k||k->n>=KU_MAX||!keys||!action) return -1;
    ku_binding *b=&k->b[k->n++]; snprintf(b->keys,KU_KEY,"%s",keys); snprintf(b->action,KU_ACT,"%s",action); b->context=context;
    return k->n-1;
}
int keymapui_find_conflict(const KeymapUi *k, const char *keys, int context) {
    if(!k||!keys) return -1;
    for (int i=0;i<k->n;i++) if (k->b[i].context==context && strcmp(k->b[i].keys,keys)==0) return i;
    return -1;
}
int keymapui_rebind(KeymapUi *k, int idx, const char *new_keys) {
    if(!k||idx<0||idx>=k->n||!new_keys) return -1;
    if (keymapui_find_conflict(k,new_keys,k->b[idx].context)>=0) return -2;
    /* conflict */
    snprintf(k->b[idx].keys,KU_KEY,"%s",new_keys); return 0;
}
const char *keymapui_action_for(const KeymapUi *k, const char *keys, int context) {
    if(!k||!keys) return 0;
    for (int i=0;i<k->n;i++) if (k->b[i].context==context && strcmp(k->b[i].keys,keys)==0) return k->b[i].action;
    return 0;
}
int keymapui_count(const KeymapUi *k) { return k?k->n:-1; }
