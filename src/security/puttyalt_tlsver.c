/* puttyalt_tlsver.c - Classify TLS protocol versions.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
const char *tv2_name(int major, int minor) {
    if (major == 3) {
        switch (minor) {
            case 0: return "SSL 3.0";
            case 1: return "TLS 1.0";
            case 2: return "TLS 1.1";
            case 3: return "TLS 1.2";
            case 4: return "TLS 1.3";
        }
    }
    return "unknown";
}
int tv2_is_secure(int major, int minor) {
    /* TLS 1.2+ considered secure */
    return major == 3 && minor >= 3;
}
int tv2_is_deprecated(int major, int minor) {
    return major == 3 && minor <= 2;
}
