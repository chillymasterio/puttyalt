/* puttyalt_keybindmap.c - Key binding map with modifier combos. */
#include <string.h>
#include <stdio.h>
#define KB_MAX 128
#define KB_MOD_CTRL 1
#define KB_MOD_ALT 2
#define KB_MOD_SHIFT 4
typedef struct { int keycode; int modifiers; int action_id; } kb_binding;
typedef struct { kb_binding bindings[KB_MAX]; int n; } KeyBindMap;
void keybindmap_init(KeyBindMap *k) { if(k) memset(k,0,sizeof(*k)); }
int keybindmap_bind(KeyBindMap *k, int keycode, int modifiers, int action_id) {
    if(!k||k->n>=KB_MAX) return -1;
    /* replace existing */
    for (int i=0;i<k->n;i++) if (k->bindings[i].keycode==keycode && k->bindings[i].modifiers==modifiers) {
        k->bindings[i].action_id=action_id; return 0;
    }
    kb_binding *b=&k->bindings[k->n++]; b->keycode=keycode; b->modifiers=modifiers; b->action_id=action_id; return 0;
}
int keybindmap_lookup(const KeyBindMap *k, int keycode, int modifiers) {
    if(!k) return -1;
    for (int i=0;i<k->n;i++) if (k->bindings[i].keycode==keycode && k->bindings[i].modifiers==modifiers) return k->bindings[i].action_id;
    return -1;
}
int keybindmap_format(int keycode, int modifiers, char *out, int outlen) {
    if(!out) return -1;
    int pos=0;
    if (modifiers&KB_MOD_CTRL) pos+=snprintf(out+pos,outlen-pos,"Ctrl+");
    if (modifiers&KB_MOD_ALT) pos+=snprintf(out+pos,outlen-pos,"Alt+");
    if (modifiers&KB_MOD_SHIFT) pos+=snprintf(out+pos,outlen-pos,"Shift+");
    if (keycode>=32 && keycode<127) pos+=snprintf(out+pos,outlen-pos,"%c",keycode);
    else pos+=snprintf(out+pos,outlen-pos,"Key%d",keycode);
    return pos;
}
int keybindmap_count(const KeyBindMap *k) { return k?k->n:-1; }
