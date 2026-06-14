/* puttyalt_slugify.c - Turn a label into a URL slug.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#include <ctype.h>
int slg_make(const char *s, char *out, int outlen) {
    if (!s || !out || outlen <= 0) return -1;
    int o = 0, prev_dash = 1; /* avoid leading dash */
    for (; *s && o < outlen - 1; s++) {
        unsigned char c = (unsigned char)*s;
        if (isalnum(c)) { out[o++] = (char)tolower(c); prev_dash = 0; }
        else if (!prev_dash) { out[o++] = '-'; prev_dash = 1; }
    }
    while (o > 0 && out[o-1] == '-') o--; /* strip trailing */
    out[o] = 0;
    return o;
}
