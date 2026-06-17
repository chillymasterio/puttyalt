/* puttyalt_rndname.c - Generate deterministic readable names.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
static const char *RN_ADJ[] = {"swift","calm","bright","quiet","bold","keen","warm","cool","brisk","lone"};
static const char *RN_NOUN[] = {"falcon","river","ember","cedar","harbor","meadow","summit","willow","canyon","beacon"};
/* Build "swift-falcon-42" from a seed deterministically. */
int rn_generate(unsigned seed, char *out, int outlen) {
    if (!out) return -1;
    unsigned a = seed % 10;
    unsigned b = (seed / 10) % 10;
    unsigned num = (seed / 100) % 100;
    int o = 0;
    const char *adj = RN_ADJ[a], *noun = RN_NOUN[b];
    while (*adj && o < outlen - 1) out[o++] = *adj++;
    if (o < outlen - 1) out[o++] = '-';
    while (*noun && o < outlen - 1) out[o++] = *noun++;
    if (o < outlen - 4) { out[o++]='-'; out[o++]=(char)('0'+num/10); out[o++]=(char)('0'+num%10); }
    out[o] = 0;
    return o;
}
