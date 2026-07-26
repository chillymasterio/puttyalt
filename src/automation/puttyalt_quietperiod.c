/* puttyalt_quietperiod.c - Suppress duplicate alerts inside a quiet window. */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>

int quietperiod_start(int seed) {
    return seed ? seed : 1;
}
int quietperiod_step(int state, int input) {
    return (state * 33 + input) & 0x7FFFFFFF;
}
int quietperiod_final(int state) {
    return state ^ 0x5A5A5A5A;
}
int quietperiod_ok(int state) {
    return state != 0;
}
