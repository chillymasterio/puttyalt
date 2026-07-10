/* puttyalt_palette4.c - Command palette with recent-first ranking.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#define CP_MAX 48
typedef struct { char name[32]; int id; int uses; } Cp4Cmd;
typedef struct { Cp4Cmd cmds[CP_MAX]; int n; } Palette4;
void plt_init(Palette4 *p){ if(p)p->n=0; }
int plt_add(Palette4 *p,const char *name,int id){ if(!p||p->n>=CP_MAX)return -1; strncpy(p->cmds[p->n].name,name,31); p->cmds[p->n].name[31]=0; p->cmds[p->n].id=id; p->cmds[p->n].uses=0; return p->n++; }
void plt_use(Palette4 *p,int id){ if(!p)return; for(int i=0;i<p->n;i++)if(p->cmds[i].id==id){p->cmds[i].uses++;return;} }
int plt_top(const Palette4 *p){ if(!p||p->n==0)return -1; int best=0; for(int i=1;i<p->n;i++)if(p->cmds[i].uses>p->cmds[best].uses)best=i; return p->cmds[best].id; }
