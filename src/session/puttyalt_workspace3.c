/* puttyalt_workspace3.c - Workspace session grouping.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#define WS_MAX 8
#define WS_SESS 16
typedef struct { char name[24]; int sessions[WS_SESS]; int n; } WsSpace;
typedef struct { WsSpace spaces[WS_MAX]; int n; int active; } Workspace3;
void wsp_init(Workspace3 *w){ if(w){w->n=0;w->active=-1;} }
int wsp_create(Workspace3 *w,const char *name){ if(!w||w->n>=WS_MAX)return -1; strncpy(w->spaces[w->n].name,name,23); w->spaces[w->n].name[23]=0; w->spaces[w->n].n=0; if(w->active<0)w->active=0; return w->n++; }
int wsp_add_session(Workspace3 *w,int ws,int sid){ if(!w||ws<0||ws>=w->n||w->spaces[ws].n>=WS_SESS)return -1; w->spaces[ws].sessions[w->spaces[ws].n++]=sid; return 0; }
