/* puttyalt_latin1conv.c - Latin-1 / ASCII transliteration.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stdint.h>
/* Convert a Latin-1 byte to a rough ASCII equivalent. */
char l1_to_ascii(uint8_t c) {
    if (c < 0x80) return (char)c;
    switch (c) {
        case 0xC0: case 0xC1: case 0xC2: case 0xC3: case 0xC4: case 0xC5: return 'A';
        case 0xE0: case 0xE1: case 0xE2: case 0xE3: case 0xE4: case 0xE5: return 'a';
        case 0xC8: case 0xC9: case 0xCA: case 0xCB: return 'E';
        case 0xE8: case 0xE9: case 0xEA: case 0xEB: return 'e';
        case 0xCC: case 0xCD: case 0xCE: case 0xCF: return 'I';
        case 0xEC: case 0xED: case 0xEE: case 0xEF: return 'i';
        case 0xD2: case 0xD3: case 0xD4: case 0xD5: case 0xD6: return 'O';
        case 0xF2: case 0xF3: case 0xF4: case 0xF5: case 0xF6: return 'o';
        case 0xD9: case 0xDA: case 0xDB: case 0xDC: return 'U';
        case 0xF9: case 0xFA: case 0xFB: case 0xFC: return 'u';
        case 0xD1: return 'N'; case 0xF1: return 'n';
        case 0xC7: return 'C'; case 0xE7: return 'c';
        default: return '?';
    }
}
int l1_transliterate(const uint8_t *in, int n, char *out, int outlen) {
    int o = 0;
    for (int i = 0; i < n && o < outlen-1; i++) out[o++] = l1_to_ascii(in[i]);
    out[o] = 0;
    return o;
}
