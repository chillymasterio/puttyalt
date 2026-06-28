/* puttyalt_permbits.c - Unix permission bit manipulation.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stdio.h>
/* Format a mode like 0755 into "rwxr-xr-x". */
int pb3_format(int mode, char *out, int outlen) {
    if (!out || outlen < 10) return -1;
    const char *rwx = "rwxrwxrwx";
    for (int i = 0; i < 9; i++)
        out[i] = (mode & (1 << (8 - i))) ? rwx[i] : '-';
    out[9] = 0;
    return 9;
}
int pb3_parse(const char *str) {
    if (!str) return -1;
    int mode = 0;
    for (int i = 0; i < 9 && str[i]; i++)
        if (str[i] != '-') mode |= (1 << (8 - i));
    return mode;
}
int pb3_octal(int mode) {
    return ((mode >> 6) & 7) * 100 + ((mode >> 3) & 7) * 10 + (mode & 7);
}
