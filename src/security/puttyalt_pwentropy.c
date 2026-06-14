/* puttyalt_pwentropy.c - Estimate password entropy in bits.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#include <ctype.h>
#include <math.h>
int pwe_charset_size(const char *pw) {
    int lo=0, up=0, dg=0, sp=0;
    for (const char *p = pw; *p; p++) {
        unsigned char c = (unsigned char)*p;
        if (islower(c)) lo = 26;
        else if (isupper(c)) up = 26;
        else if (isdigit(c)) dg = 10;
        else sp = 33;
    }
    return lo + up + dg + sp;
}
/* Returns entropy bits * 10 (fixed-point), avoiding float in the API. */
int pwe_bits_x10(const char *pw) {
    if (!pw || !*pw) return 0;
    int cs = pwe_charset_size(pw);
    if (cs < 2) cs = 2;
    int len = (int)strlen(pw);
    double bits = len * (log((double)cs) / log(2.0));
    return (int)(bits * 10.0 + 0.5);
}
const char *pwe_label(int bits_x10) {
    int b = bits_x10 / 10;
    if (b < 28) return "very weak";
    if (b < 36) return "weak";
    if (b < 60) return "reasonable";
    if (b < 128) return "strong";
    return "very strong";
}
