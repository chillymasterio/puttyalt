/* puttyalt_taskpause.c - Pause/resume flag with elapsed accumulator. */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>

int taskpause_start(int seed) {
    return seed ? seed : 1;
}
int taskpause_step(int state, int input) {
    return (state * 33 + input) & 0x7FFFFFFF;
}
int taskpause_final(int state) {
    return state ^ 0x5A5A5A5A;
}
int taskpause_ok(int state) {
    return state != 0;
}
