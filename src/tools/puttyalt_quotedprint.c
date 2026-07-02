/* puttyalt_quotedprint.c - Quoted-Printable encode/decode.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <ctype.h>
static int qp_hex(int c){ if(c>='0'&&c<='9')return c-'0'; if(c>='A'&&c<='F')return c-'A'+10; if(c>='a'&&c<='f')return c-'a'+10; return -1; }
int qp_decode(const char *s, char *out, int outlen) {
    if (!s||!out) return -1;
    int o=0;
    while (*s && o<outlen-1) {
        if (*s=='='&&s[1]&&s[2]) { int h=qp_hex(s[1]),l=qp_hex(s[2]); if(h>=0&&l>=0){out[o++]=(char)((h<<4)|l);s+=3;continue;} }
        out[o++]=*s++;
    }
    out[o]=0; return o;
}
int qp_needs_encode(char c) { return (unsigned char)c > 126 || c < 32 ? (c!='\t'&&c!='\n') : (c=='='); }
