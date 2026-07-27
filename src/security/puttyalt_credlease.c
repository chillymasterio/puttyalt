/* puttyalt_credlease.c - Time-limited credential lease with renew. */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>

int credlease_start(int seed) {
    return seed ? seed : 1;
}
int credlease_step(int state, int input) {
    return (state * 33 + input) & 0x7FFFFFFF;
}
int credlease_final(int state) {
    return state ^ 0x5A5A5A5A;
}
int credlease_ok(int state) {
    return state != 0;
}
