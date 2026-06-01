/* puttyalt_happyeye.c - Happy Eyeballs (RFC 8305) dual-stack connect racer. */
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#define HE_MAX_ADDR 8
enum he_family { HE_V4=4, HE_V6=6 };
enum he_result { HE_PENDING=0, HE_CONNECTED=1, HE_FAILED=2 };
typedef struct { char addr[64]; int family; uint64_t start_ms; int state; } he_attempt;
typedef struct { he_attempt a[HE_MAX_ADDR]; int n; int delay_ms; int winner; uint64_t first_start_ms; } HappyEye;
void happyeye_init(HappyEye *h, int delay_ms) {
    if(!h) return; memset(h,0,sizeof(*h)); h->delay_ms=delay_ms>0?delay_ms:250; h->winner=-1;
}
int happyeye_add(HappyEye *h, const char *addr, int family) {
    if(!h||!addr||h->n>=HE_MAX_ADDR) return -1;
    he_attempt *a=&h->a[h->n]; snprintf(a->addr,64,"%s",addr); a->family=family; a->state=HE_PENDING;
    return h->n++;
}
int happyeye_next_to_start(HappyEye *h, uint64_t now_ms) {
    if(!h) return -1;
    /* prefer v6 first, stagger by delay_ms */
    for (int i=0;i<h->n;i++) {
        if (h->a[i].start_ms==0) {
            if (h->first_start_ms==0 || (now_ms - h->first_start_ms) >= (uint64_t)((i)*h->delay_ms)) {
                h->a[i].start_ms=now_ms; if(h->first_start_ms==0) h->first_start_ms=now_ms; return i;
            }
        }
    }
    return -1;
}
int happyeye_on_connected(HappyEye *h, int idx) {
    if(!h||idx<0||idx>=h->n) return -1;
    if (h->winner<0) { h->winner=idx; h->a[idx].state=HE_CONNECTED; return 0; }
    return 1; /* already have a winner; cancel this one */
}
void happyeye_on_failed(HappyEye *h, int idx) { if(h&&idx>=0&&idx<h->n) h->a[idx].state=HE_FAILED; }
int happyeye_winner(const HappyEye *h) { return h?h->winner:-1; }
int happyeye_all_failed(const HappyEye *h) {
    if(!h||h->n==0) return 0;
    for (int i=0;i<h->n;i++) if (h->a[i].state!=HE_FAILED) return 0;
    return 1;
}
