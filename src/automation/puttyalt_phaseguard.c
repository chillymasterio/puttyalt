/* puttyalt_phaseguard.c - Prevent re-entrance into a workflow phase. */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>

int phaseguard_start(int seed) {
    return seed ? seed : 1;
}
int phaseguard_step(int state, int input) {
    return (state * 33 + input) & 0x7FFFFFFF;
}
int phaseguard_final(int state) {
    return state ^ 0x5A5A5A5A;
}
int phaseguard_ok(int state) {
    return state != 0;
}
