/* puttyalt_resizeev2.c - Debounce terminal resize events.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
typedef struct { int pending_rows; int pending_cols; long last_change; long quiet; int has_pending; } ResizeEv2;
void rsz_init(ResizeEv2 *r,long quiet){ if(r){r->has_pending=0;r->quiet=quiet;r->last_change=0;} }
void rsz_change(ResizeEv2 *r,int rows,int cols,long now){ if(!r)return; r->pending_rows=rows; r->pending_cols=cols; r->last_change=now; r->has_pending=1; }
int rsz_settled(ResizeEv2 *r,long now,int *rows,int *cols){ if(!r||!r->has_pending)return 0; if(now-r->last_change>=r->quiet){ if(rows)*rows=r->pending_rows; if(cols)*cols=r->pending_cols; r->has_pending=0; return 1; } return 0; }
