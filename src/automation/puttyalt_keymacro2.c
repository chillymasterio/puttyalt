/* puttyalt_keymacro2.c - Record and expand key macros.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#define KM_MAX 64
typedef struct { char keys[KM_MAX]; int n; int recording; } KeyMacro2;
void kmc_init(KeyMacro2 *m){ if(m){m->n=0;m->recording=0;} }
void kmc_start(KeyMacro2 *m){ if(m){m->n=0;m->recording=1;} }
void kmc_stop(KeyMacro2 *m){ if(m)m->recording=0; }
void kmc_key(KeyMacro2 *m,char k){ if(m&&m->recording&&m->n<KM_MAX)m->keys[m->n++]=k; }
int kmc_playback(const KeyMacro2 *m,char *out,int outlen){ if(!m)return 0; int c=m->n<outlen-1?m->n:outlen-1; for(int i=0;i<c;i++)out[i]=m->keys[i]; out[c]=0; return c; }
