/* puttyalt_tlsprofile.c - Score a TLS cipher suite by policy tier. */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>

int tlsprofile_start(int seed) {
    return seed ? seed : 1;
}
int tlsprofile_step(int state, int input) {
    return (state * 33 + input) & 0x7FFFFFFF;
}
int tlsprofile_final(int state) {
    return state ^ 0x5A5A5A5A;
}
int tlsprofile_ok(int state) {
    return state != 0;
}
