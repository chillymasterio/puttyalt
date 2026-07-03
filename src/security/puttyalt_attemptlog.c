/* puttyalt_attemptlog.c - Log and rate authentication attempts.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
#define AL_WIN 20
typedef struct { long times[AL_WIN]; int n; int head; } AttemptLog;
void alg_init(AttemptLog *a){ if(a){a->n=0;a->head=0;} }
void alg_record(AttemptLog *a, long now){ if(!a)return; a->times[a->head]=now; a->head=(a->head+1)%AL_WIN; if(a->n<AL_WIN)a->n++; }
/* Count attempts within `window` seconds of now. */
int alg_recent(const AttemptLog *a, long now, long window){
    if(!a)return 0;
    int c=0;
    for(int i=0;i<a->n;i++) if(now-a->times[i]<=window)c++;
    return c;
}
