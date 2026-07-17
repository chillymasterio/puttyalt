/* puttyalt_logring2.c - Ring buffer of structured log entries.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#define LR_CAP 32
typedef struct { long ts; int level; char msg[96]; } LrEntry;
typedef struct { LrEntry e[LR_CAP]; int head; int count; } LogRing2;
void lgr_init(LogRing2 *l){ if(l){l->head=0;l->count=0;} }
void lgr_add(LogRing2 *l,long ts,int level,const char *msg){ if(!l)return; l->e[l->head].ts=ts; l->e[l->head].level=level; strncpy(l->e[l->head].msg,msg,95); l->e[l->head].msg[95]=0; l->head=(l->head+1)%LR_CAP; if(l->count<LR_CAP)l->count++; }
int lgr_count_level(const LogRing2 *l,int min_level){ if(!l)return 0; int c=0; for(int i=0;i<l->count;i++)if(l->e[i].level>=min_level)c++; return c; }
