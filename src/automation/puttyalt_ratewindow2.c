/* puttyalt_ratewindow2.c - Fixed-window rate limiter with reset counter. */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>

int ratewindow2_start(int seed) {
    return seed ? seed : 1;
}
int ratewindow2_step(int state, int input) {
    return (state * 33 + input) & 0x7FFFFFFF;
}
int ratewindow2_final(int state) {
    return state ^ 0x5A5A5A5A;
}
int ratewindow2_ok(int state) {
    return state != 0;
}
