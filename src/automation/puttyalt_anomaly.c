/* puttyalt_anomaly.c - Statistical anomaly detector (z-score over rolling window). */
#include <string.h>
#include <stdint.h>
#define AN_WINDOW 64
typedef struct { double samples[AN_WINDOW]; int head, count; double mean, m2; double threshold_z; } Anomaly;
void anomaly_init(Anomaly *a, double threshold_z) {
    if(!a) return;
    memset(a,0,sizeof(*a)); a->threshold_z=threshold_z>0?threshold_z:3.0;
}
static double an_stddev(const Anomaly *a) {
    if (a->count<2) return 0;
    return a->m2/(a->count-1);
}
int anomaly_observe(Anomaly *a, double value) {
    if(!a) return -1;
    /* Welford online mean/variance */
    a->count = a->count<AN_WINDOW ? a->count+1 : AN_WINDOW;
    double delta = value - a->mean;
    a->mean += delta / a->count;
    a->m2 += delta * (value - a->mean);
    a->samples[a->head] = value; a->head=(a->head+1)%AN_WINDOW;
    return 0;
}
int anomaly_is_outlier(const Anomaly *a, double value) {
    if(!a||a->count<8) return 0;
    double var=an_stddev(a); if (var<=0) return 0;
    double sd = var; /* variance; use simple sqrt approx */
    double s=sd; for(int i=0;i<10;i++) s=(s + var/s)/2; /* newton sqrt */
    if (s<=0) return 0;
    double z = (value - a->mean)/s; if (z<0) z=-z;
    return z>a->threshold_z ? 1 : 0;
}
double anomaly_mean(const Anomaly *a) { return a?a->mean:0; }
int anomaly_count(const Anomaly *a) { return a?a->count:-1; }
