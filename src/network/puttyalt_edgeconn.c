/* puttyalt_edgeconn.c - Edge/IoT constrained-connection profile (low-power keepalive). */
#include <string.h>
#include <stdio.h>
#include <stdint.h>
enum ec_profile { EC_NORMAL=0, EC_LOW_POWER=1, EC_LOSSY=2, EC_METERED=3 };
typedef struct { int profile; int keepalive_sec; int max_payload; int compress; int batch_writes; uint64_t bytes_saved; } EdgeConn;
void edgeconn_init(EdgeConn *e, int profile) {
    if(!e) return; memset(e,0,sizeof(*e)); e->profile=profile;
    switch(profile) {
        case EC_LOW_POWER: e->keepalive_sec=300; e->max_payload=512; e->compress=1; e->batch_writes=1; break;
        case EC_LOSSY:     e->keepalive_sec=30;  e->max_payload=256; e->compress=1; e->batch_writes=0; break;
        case EC_METERED:   e->keepalive_sec=120; e->max_payload=1024;e->compress=1; e->batch_writes=1; break;
        default:           e->keepalive_sec=60;  e->max_payload=4096;e->compress=0; e->batch_writes=0; break;
    }
}
int edgeconn_should_batch(const EdgeConn *e, int pending_bytes) {
    if(!e||!e->batch_writes) return 0;
    return pending_bytes < e->max_payload ? 1 : 0;
}
int edgeconn_keepalive_due(const EdgeConn *e, uint64_t last_ms, uint64_t now_ms) {
    if(!e) return 0; return (now_ms-last_ms) >= (uint64_t)e->keepalive_sec*1000 ? 1 : 0;
}
void edgeconn_record_saved(EdgeConn *e, uint64_t bytes) { if(e) e->bytes_saved+=bytes; }
int edgeconn_max_payload(const EdgeConn *e) { return e?e->max_payload:-1; }
