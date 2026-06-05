/* puttyalt_palettecmd.c - Command palette action categories + keybind hints. */
#include <string.h>
#include <stdio.h>
#define PC_MAX 96
#define PC_NAME 64
#define PC_CAT 24
typedef struct { char label[PC_NAME]; char category[PC_CAT]; char keybind[16]; int action_id; } pc_cmd;
typedef struct { pc_cmd c[PC_MAX]; int n; } PaletteCmd;
void palettecmd_init(PaletteCmd *p) { if(p) memset(p,0,sizeof(*p)); }
int palettecmd_add(PaletteCmd *p, const char *label, const char *category, const char *keybind, int action_id) {
    if(!p||p->n>=PC_MAX||!label) return -1;
    pc_cmd *c=&p->c[p->n++]; snprintf(c->label,PC_NAME,"%s",label); snprintf(c->category,PC_CAT,"%s",category?category:"General");
    snprintf(c->keybind,16,"%s",keybind?keybind:""); c->action_id=action_id; return 0;
}
int palettecmd_by_category(const PaletteCmd *p, const char *category, int *out_ids, int cap) {
    if(!p||!category) return -1; int n=0;
    for (int i=0;i<p->n && n<cap;i++) if (strcmp(p->c[i].category,category)==0) out_ids[n++]=p->c[i].action_id;
    return n;
}
int palettecmd_keybind_for(const PaletteCmd *p, int action_id, char *out, int outlen) {
    if(!p) return -1;
    for (int i=0;i<p->n;i++) if (p->c[i].action_id==action_id) { snprintf(out,outlen,"%s",p->c[i].keybind); return 0; }
    return -1;
}
int palettecmd_count(const PaletteCmd *p) { return p?p->n:-1; }
