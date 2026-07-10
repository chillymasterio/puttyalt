/* puttyalt_mru4.c - Most-recently-used ordering list.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
#define MR_MAX 32
typedef struct { int items[MR_MAX]; int n; } Mru4;
void mru_init(Mru4 *m){ if(m)m->n=0; }
void mru_touch(Mru4 *m,int id){ if(!m)return; int pos=-1; for(int i=0;i<m->n;i++)if(m->items[i]==id){pos=i;break;} if(pos>=0){for(int i=pos;i>0;i--)m->items[i]=m->items[i-1];} else {if(m->n<MR_MAX)m->n++; for(int i=m->n-1;i>0;i--)m->items[i]=m->items[i-1];} m->items[0]=id; }
int mru_at(const Mru4 *m,int i){ return (m&&i>=0&&i<m->n)?m->items[i]:-1; }
