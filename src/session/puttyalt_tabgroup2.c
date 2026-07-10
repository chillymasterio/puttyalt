/* puttyalt_tabgroup2.c - Group tabs by color label.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#define TG_MAX 32
typedef struct { int tab_id; int group; } TgEntry;
typedef struct { TgEntry e[TG_MAX]; int n; } TabGroup2;
void tgp_init(TabGroup2 *t){ if(t)t->n=0; }
int tgp_assign(TabGroup2 *t,int tab_id,int group){ if(!t)return -1; for(int i=0;i<t->n;i++)if(t->e[i].tab_id==tab_id){t->e[i].group=group;return 0;} if(t->n>=TG_MAX)return -1; t->e[t->n].tab_id=tab_id; t->e[t->n].group=group; t->n++; return 0; }
int tgp_members(const TabGroup2 *t,int group,int *out,int cap){ if(!t)return 0; int c=0; for(int i=0;i<t->n&&c<cap;i++)if(t->e[i].group==group)out[c++]=t->e[i].tab_id; return c; }
