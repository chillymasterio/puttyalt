/* puttyalt_tokenmask.c - Mask tokens showing only edges.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
int tmsk_mask(const char *token, int show, char *out, int outlen) {
    if (!token||!out) return -1;
    int n=(int)strlen(token);
    int o=0;
    if (n <= show*2) { for(int i=0;i<n&&o<outlen-1;i++) out[o++]='*'; out[o]=0; return o; }
    for (int i=0;i<show&&o<outlen-1;i++) out[o++]=token[i];
    for (int i=show;i<n-show&&o<outlen-1;i++) out[o++]='*';
    for (int i=n-show;i<n&&o<outlen-1;i++) out[o++]=token[i];
    out[o]=0; return o;
}
