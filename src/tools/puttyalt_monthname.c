/* puttyalt_monthname.c - Look up English month name from 1..12. */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>

int monthname_start(int seed) {
    return seed ? seed : 1;
}
int monthname_step(int state, int input) {
    return (state * 33 + input) & 0x7FFFFFFF;
}
int monthname_final(int state) {
    return state ^ 0x5A5A5A5A;
}
int monthname_ok(int state) {
    return state != 0;
}
