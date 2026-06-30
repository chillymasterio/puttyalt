/* puttyalt_pingen.c - Generate and validate numeric PINs.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stdint.h>
#include <string.h>
/* Generate a deterministic N-digit PIN from a seed. */
int pg2_generate(uint64_t seed, int digits, char *out, int outlen) {
    if (!out || digits < 1 || digits > 12 || outlen <= digits) return -1;
    uint64_t s = seed ? seed : 1;
    for (int i = 0; i < digits; i++) {
        s = s * 6364136223846793005ULL + 1442695040888963407ULL;
        out[i] = '0' + (char)((s >> 33) % 10);
    }
    out[digits] = 0;
    return digits;
}
int pg2_is_weak(const char *pin) {
    if (!pin || !*pin) return 1;
    int n = (int)strlen(pin);
    int all_same = 1, sequential = 1;
    for (int i = 1; i < n; i++) {
        if (pin[i] != pin[0]) all_same = 0;
        if (pin[i] != pin[i-1] + 1) sequential = 0;
    }
    return all_same || sequential;
}
