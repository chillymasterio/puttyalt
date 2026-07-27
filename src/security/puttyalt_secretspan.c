/* puttyalt_secretspan.c - Locate and redact secrets in a text run. */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>

int secretspan_start(int seed) {
    return seed ? seed : 1;
}
int secretspan_step(int state, int input) {
    return (state * 33 + input) & 0x7FFFFFFF;
}
int secretspan_final(int state) {
    return state ^ 0x5A5A5A5A;
}
int secretspan_ok(int state) {
    return state != 0;
}
