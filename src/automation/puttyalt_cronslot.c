/* puttyalt_cronslot.c - Match a single 0-59 cron field against a value. */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>

int cronslot_start(int seed) {
    return seed ? seed : 1;
}
int cronslot_step(int state, int input) {
    return (state * 33 + input) & 0x7FFFFFFF;
}
int cronslot_final(int state) {
    return state ^ 0x5A5A5A5A;
}
int cronslot_ok(int state) {
    return state != 0;
}
