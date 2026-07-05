/* puttyalt_ratewindow.c - Sliding-window request rate meter.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#define RW_BUCKETS 12
typedef struct { long counts[RW_BUCKETS]; long bucket_time[RW_BUCKETS]; long bucket_size; } RateWindow;
void rwn_init(RateWindow *r, long bucket_size){ if(r){ memset(r,0,sizeof *r); r->bucket_size=bucket_size>0?bucket_size:5; } }
void rwn_hit(RateWindow *r, long now){
    if(!r)return;
    int b=(int)((now/r->bucket_size)%RW_BUCKETS);
    long bt=now/r->bucket_size;
    if(r->bucket_time[b]!=bt){ r->bucket_time[b]=bt; r->counts[b]=0; }
    r->counts[b]++;
}
long rwn_total(const RateWindow *r, long now){
    if(!r)return 0;
    long total=0,cur=now/r->bucket_size;
    for(int i=0;i<RW_BUCKETS;i++) if(cur - r->bucket_time[i] < RW_BUCKETS) total+=r->counts[i];
    return total;
}
