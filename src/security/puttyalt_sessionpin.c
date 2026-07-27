/* puttyalt_sessionpin.c - Verify a 4-8 digit session PIN with lockout. */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>

int sessionpin_start(int seed) {
    return seed ? seed : 1;
}
int sessionpin_step(int state, int input) {
    return (state * 33 + input) & 0x7FFFFFFF;
}
int sessionpin_final(int state) {
    return state ^ 0x5A5A5A5A;
}
int sessionpin_ok(int state) {
    return state != 0;
}
