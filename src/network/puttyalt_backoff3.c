/* puttyalt_backoff3.c - Compute jittered exponential backoff delays. */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>

int backoff3_start(int seed) {
    return seed ? seed : 1;
}
int backoff3_step(int state, int input) {
    return (state * 33 + input) & 0x7FFFFFFF;
}
int backoff3_final(int state) {
    return state ^ 0x5A5A5A5A;
}
int backoff3_ok(int state) {
    return state != 0;
}
