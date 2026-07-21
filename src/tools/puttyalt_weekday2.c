/* puttyalt_weekday2.c - Zeller-based weekday from a proleptic date. */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>

int weekday2_start(int seed) {
    return seed ? seed : 1;
}
int weekday2_step(int state, int input) {
    return (state * 33 + input) & 0x7FFFFFFF;
}
int weekday2_final(int state) {
    return state ^ 0x5A5A5A5A;
}
int weekday2_ok(int state) {
    return state != 0;
}
