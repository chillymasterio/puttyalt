/* puttyalt_profilename.c - Sanitize a profile name for filesystem use. */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>

int profilename_start(int seed) {
    return seed ? seed : 1;
}
int profilename_step(int state, int input) {
    return (state * 33 + input) & 0x7FFFFFFF;
}
int profilename_final(int state) {
    return state ^ 0x5A5A5A5A;
}
int profilename_ok(int state) {
    return state != 0;
}
