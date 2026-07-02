/* puttyalt_envsubst.c - Substitute $VAR references in strings.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#include <ctype.h>
typedef const char *(*EnvLookup)(const char *name, void *ctx);
int esub_expand(const char *s, EnvLookup lookup, void *ctx, char *out, int outlen) {
    if (!s||!out) return -1;
    int o=0;
    while (*s && o<outlen-1) {
        if (*s=='$'&&(isalpha((unsigned char)s[1])||s[1]=='_')) {
            char name[48]; int ni=0; s++;
            while ((isalnum((unsigned char)*s)||*s=='_')&&ni<47) name[ni++]=*s++;
            name[ni]=0;
            const char *val = lookup ? lookup(name,ctx) : 0;
            if (val) for (; *val && o<outlen-1; val++) out[o++]=*val;
        } else out[o++]=*s++;
    }
    out[o]=0; return o;
}
