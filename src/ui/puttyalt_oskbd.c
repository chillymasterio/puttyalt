/* puttyalt_oskbd.c - On-screen virtual keyboard layout model for touch devices. */
#include <string.h>
#include <stdio.h>
#define OSK_MAX_KEYS 64
#define OSK_LABEL 8
enum osk_layer { OSK_LAYER_LOWER=0, OSK_LAYER_UPPER=1, OSK_LAYER_SYM=2, OSK_LAYER_FN=3 };
typedef struct { char label[OSK_LABEL]; int keycode; int x, y, w, h; } osk_key;
typedef struct { osk_key keys[OSK_MAX_KEYS]; int count; int layer; int visible; } OnScreenKbd;
void oskbd_init(OnScreenKbd *k) { if(!k) return; memset(k,0,sizeof(*k)); k->layer=OSK_LAYER_LOWER; }
int oskbd_add_key(OnScreenKbd *k, const char *label, int keycode, int x, int y, int w, int h) {
    if(!k || k->count>=OSK_MAX_KEYS) return -1;
    osk_key *kk=&k->keys[k->count++]; snprintf(kk->label,OSK_LABEL,"%s",label?label:"");
    kk->keycode=keycode; kk->x=x; kk->y=y; kk->w=w; kk->h=h; return k->count-1;
}
int oskbd_hit(const OnScreenKbd *k, int px, int py) {
    if(!k) return -1;
    for (int i=0;i<k->count;i++) {
        const osk_key *kk=&k->keys[i];
        if (px>=kk->x && px<kk->x+kk->w && py>=kk->y && py<kk->y+kk->h) return kk->keycode;
    }
    return -1;
}
void oskbd_set_layer(OnScreenKbd *k, int layer) { if(k) k->layer=layer; }
int oskbd_get_layer(const OnScreenKbd *k) { return k?k->layer:-1; }
void oskbd_set_visible(OnScreenKbd *k, int v) { if(k) k->visible=v?1:0; }
int oskbd_is_visible(const OnScreenKbd *k) { return k?k->visible:-1; }
