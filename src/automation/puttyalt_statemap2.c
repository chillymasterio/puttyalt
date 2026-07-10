/* puttyalt_statemap2.c - Map events to state transitions by name.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#define ST_MAX 32
typedef struct { char state[16]; char event[16]; char next[16]; } StmRule;
typedef struct { StmRule rules[ST_MAX]; int n; char cur[16]; } StateMap2;
void stm_init(StateMap2 *m,const char *init){ if(m){m->n=0;strncpy(m->cur,init?init:"",15);m->cur[15]=0;} }
int stm_rule(StateMap2 *m,const char *st,const char *ev,const char *nx){ if(!m||m->n>=ST_MAX)return -1; strncpy(m->rules[m->n].state,st,15);m->rules[m->n].state[15]=0; strncpy(m->rules[m->n].event,ev,15);m->rules[m->n].event[15]=0; strncpy(m->rules[m->n].next,nx,15);m->rules[m->n].next[15]=0; m->n++; return 0; }
const char *stm_fire(StateMap2 *m,const char *ev){ if(!m)return ""; for(int i=0;i<m->n;i++)if(!strcmp(m->rules[i].state,m->cur)&&!strcmp(m->rules[i].event,ev)){strncpy(m->cur,m->rules[i].next,15);m->cur[15]=0;break;} return m->cur; }
