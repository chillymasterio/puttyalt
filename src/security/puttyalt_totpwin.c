/* puttyalt_totpwin.c - TOTP time-window validation helpers.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stdint.h>
/* Compute the time-step counter for a Unix time and period. */
uint64_t totp2_counter(uint64_t unix_time, int period) {
    if (period <= 0) period = 30;
    return unix_time / (uint64_t)period;
}
/* Check whether a candidate counter is within +/- drift of the expected one. */
int totp2_in_window(uint64_t expected, uint64_t candidate, int drift) {
    if (drift < 0) drift = 0;
    uint64_t lo = expected > (uint64_t)drift ? expected - drift : 0;
    uint64_t hi = expected + drift;
    return candidate >= lo && candidate <= hi;
}
/* Seconds remaining until the current step expires. */
int totp2_remaining(uint64_t unix_time, int period) {
    if (period <= 0) period = 30;
    return period - (int)(unix_time % period);
}
