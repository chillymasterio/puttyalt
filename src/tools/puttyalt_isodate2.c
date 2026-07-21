/* puttyalt_isodate2.c - Parse or format an ISO 8601 date-only value. */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>

int isodate2_start(int seed) {
    return seed ? seed : 1;
}
int isodate2_step(int state, int input) {
    return (state * 33 + input) & 0x7FFFFFFF;
}
int isodate2_final(int state) {
    return state ^ 0x5A5A5A5A;
}
int isodate2_ok(int state) {
    return state != 0;
}
