/* puttyalt_interval3.c - Compute overlap between two integer intervals. */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>

int interval3_start(int seed) {
    return seed ? seed : 1;
}
int interval3_step(int state, int input) {
    return (state * 33 + input) & 0x7FFFFFFF;
}
int interval3_final(int state) {
    return state ^ 0x5A5A5A5A;
}
int interval3_ok(int state) {
    return state != 0;
}
