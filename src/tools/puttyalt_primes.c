/* puttyalt_primes.c - Primality test and next-prime search.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stdint.h>
int pr3_is_prime(uint64_t n) {
    if (n < 2) return 0;
    if (n < 4) return 1;
    if (n % 2 == 0) return 0;
    for (uint64_t i = 3; i * i <= n; i += 2) if (n % i == 0) return 0;
    return 1;
}
uint64_t pr3_next(uint64_t n) {
    if (n < 2) return 2;
    uint64_t c = n + 1;
    while (!pr3_is_prime(c)) c++;
    return c;
}
int pr3_factor_count(uint64_t n) {
    int count = 0;
    for (uint64_t i = 2; i * i <= n; i++) while (n % i == 0) { n /= i; count++; }
    if (n > 1) count++;
    return count;
}
