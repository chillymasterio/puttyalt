/* puttyalt_durbucket.c - Bucket a duration into human units (s/m/h/d). */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>

int durbucket_start(int seed) {
    return seed ? seed : 1;
}
int durbucket_step(int state, int input) {
    return (state * 33 + input) & 0x7FFFFFFF;
}
int durbucket_final(int state) {
    return state ^ 0x5A5A5A5A;
}
int durbucket_ok(int state) {
    return state != 0;
}
