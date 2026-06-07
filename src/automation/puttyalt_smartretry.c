/* puttyalt_smartretry.c - Adaptive retry strategy learning from outcomes. */
#include <string.h>
#include <stdint.h>
typedef struct { int success_count, fail_count; int base_delay_ms; int learned_delay_ms; double success_rate; } SmartRetry;
void smartretry_init(SmartRetry *s, int base_delay_ms) {
    if(!s) return; memset(s,0,sizeof(*s)); s->base_delay_ms=base_delay_ms>0?base_delay_ms:1000;
    s->learned_delay_ms=s->base_delay_ms;
}
int smartretry_record(SmartRetry *s, int success, int actual_delay_ms) {
    if(!s) return -1;
    if (success) {
        s->success_count++;
        /* if it succeeded quickly, reduce delay */
        if (actual_delay_ms < s->learned_delay_ms) s->learned_delay_ms = (s->learned_delay_ms*3 + actual_delay_ms)/4;
    } else {
        s->fail_count++;
        /* on failure, increase delay */
        s->learned_delay_ms = (int)(s->learned_delay_ms * 1.5);
        if (s->learned_delay_ms > s->base_delay_ms*30) s->learned_delay_ms = s->base_delay_ms*30;
    }
    int total=s->success_count+s->fail_count;
    s->success_rate = total>0 ? (double)s->success_count/total : 0;
    return 0;
}
int smartretry_next_delay(const SmartRetry *s) { return s?s->learned_delay_ms:-1; }
int smartretry_should_give_up(const SmartRetry *s) {
    if(!s) return 1;
    int total=s->success_count+s->fail_count;
    return (total>=10 && s->success_rate<0.1) ? 1 : 0;
}
double smartretry_success_rate(const SmartRetry *s) { return s?s->success_rate:0; }
