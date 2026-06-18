/* puttyalt_romanfmt.c - Convert integers to/from Roman numerals.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
int rm2_to_roman(int n, char *out, int outlen) {
    if (!out || n <= 0 || n > 3999) return -1;
    static const int vals[] = {1000,900,500,400,100,90,50,40,10,9,5,4,1};
    static const char *syms[] = {"M","CM","D","CD","C","XC","L","XL","X","IX","V","IV","I"};
    int o = 0;
    for (int i = 0; i < 13; i++) {
        while (n >= vals[i]) {
            for (const char *p = syms[i]; *p && o < outlen-1; p++) out[o++] = *p;
            n -= vals[i];
        }
    }
    out[o] = 0;
    return o;
}
int rm2_from_roman(const char *s) {
    if (!s) return -1;
    int total = 0, prev = 0;
    for (int i = (int)strlen(s) - 1; i >= 0; i--) {
        int v;
        switch (s[i]) {
            case 'I': v=1; break; case 'V': v=5; break; case 'X': v=10; break;
            case 'L': v=50; break; case 'C': v=100; break; case 'D': v=500; break;
            case 'M': v=1000; break; default: return -1;
        }
        if (v < prev) total -= v; else { total += v; prev = v; }
    }
    return total;
}
