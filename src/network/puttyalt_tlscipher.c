/* puttyalt_tlscipher.c - Classify TLS cipher suite strength.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
/* Rate a cipher suite name. Returns 0=insecure 1=weak 2=ok 3=strong. */
int tc4_rate(const char *cipher) {
    if (!cipher) return 0;
    if (strstr(cipher, "NULL") || strstr(cipher, "EXPORT") || strstr(cipher, "anon")) return 0;
    if (strstr(cipher, "RC4") || strstr(cipher, "DES") || strstr(cipher, "MD5")) return 0;
    if (strstr(cipher, "CBC") && strstr(cipher, "SHA")) return 1;
    if (strstr(cipher, "GCM") || strstr(cipher, "CHACHA20")) {
        if (strstr(cipher, "ECDHE")) return 3;
        return 2;
    }
    return 1;
}
int tc4_has_forward_secrecy(const char *cipher) {
    return cipher && (strstr(cipher, "ECDHE") || strstr(cipher, "DHE"));
}
