/* puttyalt_topk.c - Track top-K largest values.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
#define TK_MAX 16
typedef struct { int vals[TK_MAX]; int k; int n; } TopK;
void tpk_init(TopK *t,int k){ if(t){t->k=k>TK_MAX?TK_MAX:k;t->n=0;} }
void tpk_add(TopK *t,int v){ if(!t)return; if(t->n<t->k){ int i=t->n++; while(i>0&&t->vals[i-1]<v){t->vals[i]=t->vals[i-1];i--;} t->vals[i]=v; } else if(v>t->vals[t->k-1]){ int i=t->k-1; while(i>0&&t->vals[i-1]<v){t->vals[i]=t->vals[i-1];i--;} t->vals[i]=v; } }
int tpk_get(const TopK *t,int i){ return (t&&i>=0&&i<t->n)?t->vals[i]:-1; }
