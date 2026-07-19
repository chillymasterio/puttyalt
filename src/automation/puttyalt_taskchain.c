/* puttyalt_taskchain.c - Chain tasks with success/failure branches.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
#define TC_MAX 16
typedef struct { int id; int on_success; int on_failure; } TcTask;
typedef struct { TcTask tasks[TC_MAX]; int n; int current; } TaskChain;
void tsc_init(TaskChain *t){ if(t){t->n=0;t->current=0;} }
int tsc_add(TaskChain *t,int id,int on_success,int on_failure){ if(!t||t->n>=TC_MAX)return -1; t->tasks[t->n].id=id; t->tasks[t->n].on_success=on_success; t->tasks[t->n].on_failure=on_failure; return t->n++; }
int tsc_next(const TaskChain *t,int current_idx,int success){ if(!t||current_idx<0||current_idx>=t->n)return -1; return success?t->tasks[current_idx].on_success:t->tasks[current_idx].on_failure; }
