/* puttyalt_hashset2.c - Open-addressing integer hash set.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
#define HS_CAP 128
typedef struct { int keys[HS_CAP]; unsigned char used[HS_CAP]; int count; } HashSet2;
void hst2_init(HashSet2 *s){ if(!s)return; s->count=0; for(int i=0;i<HS_CAP;i++)s->used[i]=0; }
int hst2_add(HashSet2 *s,int key){ if(!s||s->count>=HS_CAP)return -1; unsigned h=((unsigned)key*2654435761u)%HS_CAP; for(int i=0;i<HS_CAP;i++){ int idx=(h+i)%HS_CAP; if(!s->used[idx]){s->used[idx]=1;s->keys[idx]=key;s->count++;return 0;} if(s->keys[idx]==key)return 1; } return -1; }
int hst2_contains(const HashSet2 *s,int key){ if(!s)return 0; unsigned h=((unsigned)key*2654435761u)%HS_CAP; for(int i=0;i<HS_CAP;i++){ int idx=(h+i)%HS_CAP; if(!s->used[idx])return 0; if(s->keys[idx]==key)return 1; } return 0; }
