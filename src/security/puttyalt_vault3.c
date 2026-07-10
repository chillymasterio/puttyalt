/* puttyalt_vault3.c - In-memory secret vault with lookup.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#define VT_MAX 24
typedef struct { char name[32]; char value[96]; int locked; } VtEntry;
typedef struct { VtEntry e[VT_MAX]; int n; int unlocked; } Vault3;
void vlt_init(Vault3 *v){ if(v){v->n=0;v->unlocked=0;} }
void vlt_unlock(Vault3 *v,int ok){ if(v)v->unlocked=ok?1:0; }
int vlt_store(Vault3 *v,const char *name,const char *value){ if(!v)return -1; if(v->n>=VT_MAX)return -1; strncpy(v->e[v->n].name,name,31);v->e[v->n].name[31]=0; strncpy(v->e[v->n].value,value,95);v->e[v->n].value[95]=0; v->e[v->n].locked=1; v->n++; return 0; }
const char *vlt_get(const Vault3 *v,const char *name){ if(!v||!v->unlocked)return 0; for(int i=0;i<v->n;i++)if(!strcmp(v->e[i].name,name))return v->e[i].value; return 0; }
