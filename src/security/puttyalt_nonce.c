/* puttyalt_nonce.c - Generate and track unique nonces.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stdint.h>
#define NNC_HISTORY 64
typedef struct { uint64_t seen[NNC_HISTORY]; int n; uint64_t counter; } NonceTracker;
void nnc_init(NonceTracker *t, uint64_t seed) { if (t) { t->n = 0; t->counter = seed; } }
/* Generate a new monotonic nonce. */
uint64_t nnc_generate(NonceTracker *t) {
    if (!t) return 0;
    t->counter = t->counter * 6364136223846793005ULL + 1442695040888963407ULL;
    return t->counter;
}
/* Check + record a nonce; returns 1 if fresh, 0 if replayed. */
int nnc_check(NonceTracker *t, uint64_t nonce) {
    if (!t) return 0;
    for (int i = 0; i < t->n; i++) if (t->seen[i] == nonce) return 0;
    if (t->n < NNC_HISTORY) t->seen[t->n++] = nonce;
    else { for (int i = 1; i < NNC_HISTORY; i++) t->seen[i-1] = t->seen[i]; t->seen[NNC_HISTORY-1] = nonce; }
    return 1;
}
