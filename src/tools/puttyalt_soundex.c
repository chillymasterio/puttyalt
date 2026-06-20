/* puttyalt_soundex.c - Soundex phonetic encoding.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <ctype.h>
#include <string.h>
static char sx2_code(char c) {
    switch (toupper((unsigned char)c)) {
        case 'B': case 'F': case 'P': case 'V': return '1';
        case 'C': case 'G': case 'J': case 'K': case 'Q': case 'S': case 'X': case 'Z': return '2';
        case 'D': case 'T': return '3';
        case 'L': return '4';
        case 'M': case 'N': return '5';
        case 'R': return '6';
        default: return '0';
    }
}
int sx2_encode(const char *s, char *out, int outlen) {
    if (!s || !out || outlen < 5) return -1;
    while (*s && !isalpha((unsigned char)*s)) s++;
    if (!*s) { out[0] = 0; return 0; }
    out[0] = (char)toupper((unsigned char)*s);
    char prev = sx2_code(*s);
    int o = 1;
    for (s++; *s && o < 4; s++) {
        char c = sx2_code(*s);
        if (c != '0' && c != prev) out[o++] = c;
        if (c != '0' || (toupper((unsigned char)*s) != 'H' && toupper((unsigned char)*s) != 'W')) prev = c;
    }
    while (o < 4) out[o++] = '0';
    out[o] = 0;
    return o;
}
