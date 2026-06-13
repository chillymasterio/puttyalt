/* puttyalt_numfmt.c - Number formatting (ordinal, roman, percent, fraction). */
#include <string.h>
#include <stdio.h>
int numfmt_ordinal(int n, char *out, int outlen) {
    if (!out) return -1;
    const char *suffix = "th";
    int mod100 = n % 100;
    if (mod100 < 11 || mod100 > 13) {
        switch (n % 10) {
            case 1: suffix = "st"; break;
            case 2: suffix = "nd"; break;
            case 3: suffix = "rd"; break;
        }
    }
    return snprintf(out, outlen, "%d%s", n, suffix);
}
int numfmt_roman(int n, char *out, int outlen) {
    if (!out || n <= 0 || n > 3999) return -1;
    static const int vals[] = {1000, 900, 500, 400, 100, 90, 50, 40, 10, 9, 5, 4, 1};
    static const char *syms[] = {"M", "CM", "D", "CD", "C", "XC", "L", "XL", "X", "IX", "V", "IV", "I"};
    int pos = 0;
    for (int i = 0; i < 13; i++) {
        while (n >= vals[i] && pos < outlen - 3) {
            pos += snprintf(out + pos, outlen - pos, "%s", syms[i]);
            n -= vals[i];
        }
    }
    return pos;
}
int numfmt_percent(int numerator, int denominator, char *out, int outlen) {
    if (!out || denominator == 0) return -1;
    int pct = numerator * 100 / denominator;
    return snprintf(out, outlen, "%d%%", pct);
}
