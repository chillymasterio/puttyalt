/* puttyalt_sessfolder.c - Hierarchical session folder tree with collapse state. */
#include <string.h>
#include <stdio.h>
#define SF_MAX 64
#define SF_NAME 48
typedef struct { char name[SF_NAME]; int parent; int collapsed; int is_session; int item_id; } sf_node;
typedef struct { sf_node n[SF_MAX]; int count; } SessFolder;
void sessfolder_init(SessFolder *f) { if(f){ memset(f,0,sizeof(*f)); } }
int sessfolder_add_folder(SessFolder *f, const char *name, int parent) {
    if(!f||f->count>=SF_MAX||!name) return -1;
    if (parent>=f->count) return -1;
    sf_node *n=&f->n[f->count]; snprintf(n->name,SF_NAME,"%s",name); n->parent=parent; n->is_session=0; n->item_id=-1;
    return f->count++;
}
int sessfolder_add_session(SessFolder *f, const char *name, int parent, int session_id) {
    if(!f||f->count>=SF_MAX||!name) return -1;
    sf_node *n=&f->n[f->count]; snprintf(n->name,SF_NAME,"%s",name); n->parent=parent; n->is_session=1; n->item_id=session_id;
    return f->count++;
}
void sessfolder_toggle(SessFolder *f, int idx) { if(f&&idx>=0&&idx<f->count&&!f->n[idx].is_session) f->n[idx].collapsed^=1; }
int sessfolder_is_visible(const SessFolder *f, int idx) {
    if(!f||idx<0||idx>=f->count) return 0;
    int p=f->n[idx].parent, guard=0;
    while (p>=0 && guard++<SF_MAX) { if (f->n[p].collapsed) return 0; p=f->n[p].parent; }
    return 1;
}
int sessfolder_depth(const SessFolder *f, int idx) {
    if(!f||idx<0||idx>=f->count) return -1;
    int d=0, p=f->n[idx].parent, guard=0; while (p>=0 && guard++<SF_MAX) { d++; p=f->n[p].parent; } return d;
}
int sessfolder_count(const SessFolder *f) { return f?f->count:-1; }
