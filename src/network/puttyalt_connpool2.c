/* puttyalt_connpool2.c - Connection pool slot manager.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
#define CP_MAX 24
typedef struct { int id; int busy; long since; } CpConn;
typedef struct { CpConn c[CP_MAX]; int size; } ConnPool2;
void cpl_init(ConnPool2 *p, int size){ if(p){ p->size=size>CP_MAX?CP_MAX:size; for(int i=0;i<p->size;i++){p->c[i].id=i;p->c[i].busy=0;} } }
int cpl_acquire(ConnPool2 *p, long now){ if(!p)return -1; for(int i=0;i<p->size;i++) if(!p->c[i].busy){ p->c[i].busy=1; p->c[i].since=now; return i; } return -1; }
void cpl_release(ConnPool2 *p, int id){ if(p&&id>=0&&id<p->size)p->c[id].busy=0; }
int cpl_busy(const ConnPool2 *p){ if(!p)return 0; int c=0; for(int i=0;i<p->size;i++)if(p->c[i].busy)c++; return c; }
