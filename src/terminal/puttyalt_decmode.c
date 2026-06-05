/* puttyalt_decmode.c - DEC private mode (DECSET/DECRST) registry. */
#include <string.h>
#define DM_MAX 32
typedef struct { int mode; int enabled; } dm_entry;
typedef struct { dm_entry m[DM_MAX]; int n; } DecMode;
void decmode_init(DecMode *d) { if(d) memset(d,0,sizeof(*d)); }
int decmode_set(DecMode *d, int mode, int enabled) {
    if(!d) return -1;
    for (int i=0;i<d->n;i++) if (d->m[i].mode==mode) { d->m[i].enabled=enabled?1:0; return 0; }
    if (d->n>=DM_MAX) return -1;
    d->m[d->n].mode=mode; d->m[d->n].enabled=enabled?1:0; d->n++; return 0;
}
int decmode_get(const DecMode *d, int mode) {
    if(!d) return 0;
    for (int i=0;i<d->n;i++) if (d->m[i].mode==mode) return d->m[i].enabled;
    return 0;
}
/* Common modes: 1=app cursor, 25=cursor vis, 1000=mouse, 1049=alt screen, 2004=bracketed, 2026=sync */
int decmode_is_mouse_enabled(const DecMode *d) {
    return decmode_get(d,1000)||decmode_get(d,1002)||decmode_get(d,1003)||decmode_get(d,1006);
}
int decmode_count(const DecMode *d) { return d?d->n:-1; }
