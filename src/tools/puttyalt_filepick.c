/* puttyalt_filepick.c - File picker model with filtering + multi-select. */
#include <string.h>
#include <stdio.h>
#define FP_MAX 128
#define FP_NAME 96
typedef struct { char name[FP_NAME]; int is_dir; long size; int selected; } fp_entry;
typedef struct { fp_entry entries[FP_MAX]; int n; char filter[32]; int cursor; } FilePick;
void filepick_init(FilePick *f) { if(f) memset(f,0,sizeof(*f)); }
int filepick_add(FilePick *f, const char *name, int is_dir, long size) {
    if(!f||f->n>=FP_MAX||!name) return -1;
    fp_entry *e=&f->entries[f->n++]; snprintf(e->name,FP_NAME,"%s",name); e->is_dir=is_dir?1:0; e->size=size;
    return 0;
}
void filepick_set_filter(FilePick *f, const char *ext) { if(f) snprintf(f->filter,32,"%s",ext?ext:""); }
int filepick_visible(const FilePick *f, int idx) {
    if(!f||idx<0||idx>=f->n) return 0;
    if (f->entries[idx].is_dir) return 1; /* always show dirs */
    if (!f->filter[0]) return 1;
    const char *dot=strrchr(f->entries[idx].name,'.');
    return (dot && strcmp(dot+1,f->filter)==0)?1:0;
}
void filepick_toggle(FilePick *f, int idx) { if(f&&idx>=0&&idx<f->n&&!f->entries[idx].is_dir) f->entries[idx].selected^=1; }
int filepick_selected(const FilePick *f, int *out_idx, int cap) {
    if(!f) return -1; int n=0;
    for (int i=0;i<f->n && n<cap;i++) if (f->entries[i].selected) out_idx[n++]=i;
    return n;
}
int filepick_count(const FilePick *f) { return f?f->n:-1; }
