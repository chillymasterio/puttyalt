/* puttyalt_eventbus3.c - Topic-based event subscriber registry.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#define EB_MAX 32
typedef struct { char topic[24]; int subscriber; } EbSub;
typedef struct { EbSub subs[EB_MAX]; int n; } EventBus3;
void ebs_init(EventBus3 *b){ if(b)b->n=0; }
int ebs_subscribe(EventBus3 *b,const char *topic,int sub){ if(!b||b->n>=EB_MAX)return -1; strncpy(b->subs[b->n].topic,topic,23); b->subs[b->n].topic[23]=0; b->subs[b->n].subscriber=sub; return b->n++; }
int ebs_publish(const EventBus3 *b,const char *topic,int *out,int cap){ if(!b)return 0; int c=0; for(int i=0;i<b->n&&c<cap;i++)if(!strcmp(b->subs[i].topic,topic))out[c++]=b->subs[i].subscriber; return c; }
