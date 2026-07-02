/* puttyalt_inputmask.c - Apply input masks to text fields.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <ctype.h>
#include <stddef.h>
/* Apply a mask where '#'=digit, 'A'=letter, '*'=any. Returns formatted length. */
int imk_apply(const char *mask, const char *input, char *out, int outlen) {
    if (!mask||!input||!out) return -1;
    int o=0; const char *ip=input;
    for (const char *m=mask; *m && o<outlen-1; m++) {
        if (*m=='#'||*m=='A'||*m=='*') {
            while (*ip) {
                if ((*m=='#'&&isdigit((unsigned char)*ip)) || (*m=='A'&&isalpha((unsigned char)*ip)) || *m=='*') { out[o++]=*ip++; break; }
                ip++;
            }
            if (!*ip && (o==0||out[o-1]==0)) break;
        } else out[o++]=*m;
    }
    out[o]=0; return o;
}
