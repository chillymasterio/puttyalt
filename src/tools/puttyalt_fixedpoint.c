/* puttyalt_fixedpoint.c - 16.16 fixed-point arithmetic.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stdint.h>
typedef int32_t fp_t;
#define FP_SHIFT 16
#define FP_ONE (1 << FP_SHIFT)
fp_t fp_from_int(int i) { return (fp_t)(i << FP_SHIFT); }
int fp_to_int(fp_t v) { return v >> FP_SHIFT; }
fp_t fp_mul(fp_t a, fp_t b) { return (fp_t)(((int64_t)a * b) >> FP_SHIFT); }
fp_t fp_div(fp_t a, fp_t b) { if (b == 0) return 0; return (fp_t)(((int64_t)a << FP_SHIFT) / b); }
int fp_round(fp_t v) { return (v + (FP_ONE >> 1)) >> FP_SHIFT; }
