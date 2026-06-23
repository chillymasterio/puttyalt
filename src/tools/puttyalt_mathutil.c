/* puttyalt_mathutil.c - Misc math: factorial, fib, binomial.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stdint.h>
uint64_t mu2_factorial(int n) {
    if (n < 0 || n > 20) return 0;
    uint64_t r = 1;
    for (int i = 2; i <= n; i++) r *= i;
    return r;
}
uint64_t mu2_fib(int n) {
    if (n < 0) return 0;
    uint64_t a = 0, b = 1;
    for (int i = 0; i < n; i++) { uint64_t t = a + b; a = b; b = t; }
    return a;
}
uint64_t mu2_binomial(int n, int k) {
    if (k < 0 || k > n) return 0;
    if (k > n - k) k = n - k;
    uint64_t r = 1;
    for (int i = 0; i < k; i++) { r = r * (n - i) / (i + 1); }
    return r;
}
