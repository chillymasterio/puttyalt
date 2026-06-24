/* puttyalt_punycode.c - Basic Punycode label detection.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
/* Detect an IDNA "xn--" ACE prefix label. */
int pny_is_ace(const char *label) {
    return label && label[0]=='x' && label[1]=='n' && label[2]=='-' && label[3]=='-';
}
/* Check whether a hostname contains any non-ASCII bytes (needs IDNA). */
int pny_needs_idna(const char *host) {
    if (!host) return 0;
    for (const char *p = host; *p; p++) if ((unsigned char)*p >= 0x80) return 1;
    return 0;
}
/* Count ACE labels in a dotted hostname. */
int pny_ace_count(const char *host) {
    if (!host) return 0;
    int count = 0;
    const char *p = host;
    while (p && *p) {
        if (pny_is_ace(p)) count++;
        const char *dot = strchr(p, '.');
        if (!dot) break;
        p = dot + 1;
    }
    return count;
}
