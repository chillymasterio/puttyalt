/* puttyalt_bookmark3.c - Session bookmarks with quick access keys.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#define BM_MAX 32
typedef struct { char key; int session_id; char label[32]; } Bm3Entry;
typedef struct { Bm3Entry e[BM_MAX]; int n; } Bookmark3;
void bm3_init(Bookmark3 *b){ if(b)b->n=0; }
int bm3_set(Bookmark3 *b,char key,int session_id,const char *label){ if(!b)return -1; for(int i=0;i<b->n;i++)if(b->e[i].key==key){b->e[i].session_id=session_id;strncpy(b->e[i].label,label,31);b->e[i].label[31]=0;return 0;} if(b->n>=BM_MAX)return -1; b->e[b->n].key=key;b->e[b->n].session_id=session_id;strncpy(b->e[b->n].label,label,31);b->e[b->n].label[31]=0;b->n++; return 0; }
int bm3_lookup(const Bookmark3 *b,char key){ if(!b)return -1; for(int i=0;i<b->n;i++)if(b->e[i].key==key)return b->e[i].session_id; return -1; }
