/* puttyalt_inimerge2.c - Merge two INI key sets with override.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#define IM_MAX 48
typedef struct { char key[40]; char val[80]; } ImKv;
typedef struct { ImKv kv[IM_MAX]; int n; } IniSet;
void imr_init(IniSet *s){ if(s)s->n=0; }
int imr_set(IniSet *s,const char *k,const char *v){ if(!s)return -1; for(int i=0;i<s->n;i++)if(!strcmp(s->kv[i].key,k)){strncpy(s->kv[i].val,v,79);s->kv[i].val[79]=0;return 0;} if(s->n>=IM_MAX)return -1; strncpy(s->kv[s->n].key,k,39);s->kv[s->n].key[39]=0; strncpy(s->kv[s->n].val,v,79);s->kv[s->n].val[79]=0; s->n++; return 0; }
int imr_merge(IniSet *base,const IniSet *over){ if(!base||!over)return -1; for(int i=0;i<over->n;i++)imr_set(base,over->kv[i].key,over->kv[i].val); return 0; }
