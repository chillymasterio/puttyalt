/* puttyalt_fifo2.c - Byte FIFO with peek.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#define FF_CAP 512
typedef struct { unsigned char b[FF_CAP]; int head,tail,count; } Fifo2;
void ff2_init(Fifo2 *f){ if(f){f->head=f->tail=f->count=0;} }
int ff2_write(Fifo2 *f, const unsigned char *d, int n){ if(!f)return -1; int w=0; for(int i=0;i<n&&f->count<FF_CAP;i++){f->b[f->tail]=d[i];f->tail=(f->tail+1)%FF_CAP;f->count++;w++;} return w; }
int ff2_read(Fifo2 *f, unsigned char *d, int n){ if(!f)return -1; int r=0; for(int i=0;i<n&&f->count>0;i++){d[i]=f->b[f->head];f->head=(f->head+1)%FF_CAP;f->count--;r++;} return r; }
int ff2_peek(const Fifo2 *f, unsigned char *out){ if(!f||f->count==0)return -1; if(out)*out=f->b[f->head]; return 0; }
int ff2_available(const Fifo2 *f){ return f?f->count:0; }
