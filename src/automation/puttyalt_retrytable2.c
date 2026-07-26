/* puttyalt_retrytable2.c - Per-key exponential retry table with cap. */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>

int retrytable2_start(int seed) {
    return seed ? seed : 1;
}
int retrytable2_step(int state, int input) {
    return (state * 33 + input) & 0x7FFFFFFF;
}
int retrytable2_final(int state) {
    return state ^ 0x5A5A5A5A;
}
int retrytable2_ok(int state) {
    return state != 0;
}
