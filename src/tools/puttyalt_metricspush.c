/* puttyalt_metricspush.c - Metrics push formatter (Prometheus/StatsD line protocol). */
#include <string.h>
#include <stdio.h>
enum mp_fmt { MP_PROMETHEUS=0, MP_STATSD=1, MP_INFLUX=2 };
int metricspush_counter(int fmt, const char *name, long value, char *out, int outlen) {
    if(!name||!out) return -1;
    switch(fmt) {
        case MP_STATSD: return snprintf(out,outlen,"%s:%ld|c",name,value);
        case MP_INFLUX: return snprintf(out,outlen,"%s value=%ld",name,value);
        default: return snprintf(out,outlen,"%s %ld",name,value);
    }
}
int metricspush_gauge(int fmt, const char *name, double value, char *out, int outlen) {
    if(!name||!out) return -1;
    switch(fmt) {
        case MP_STATSD: return snprintf(out,outlen,"%s:%.2f|g",name,value);
        case MP_INFLUX: return snprintf(out,outlen,"%s value=%.2f",name,value);
        default: return snprintf(out,outlen,"%s %.2f",name,value);
    }
}
int metricspush_timing(int fmt, const char *name, int ms, char *out, int outlen) {
    if(!name||!out) return -1;
    switch(fmt) {
        case MP_STATSD: return snprintf(out,outlen,"%s:%d|ms",name,ms);
        case MP_INFLUX: return snprintf(out,outlen,"%s duration_ms=%d",name,ms);
        default: return snprintf(out,outlen,"%s_ms %d",name,ms);
    }
}
