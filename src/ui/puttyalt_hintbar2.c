/* puttyalt_hintbar2.c - Contextual hint bar builder.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stdio.h>
#include <string.h>
typedef struct { char buf[256]; int len; } HintBar2;
void hnb_init(HintBar2 *h){ if(h){h->buf[0]=0;h->len=0;} }
int hnb_add(HintBar2 *h,const char *key,const char *action){
    if(!h||!key||!action)return -1;
    int n=snprintf(h->buf+h->len,sizeof(h->buf)-h->len,"%s%s %s",h->len?"  ":"",key,action);
    if(n<0||h->len+n>=(int)sizeof h->buf)return -1;
    h->len+=n; return 0;
}
