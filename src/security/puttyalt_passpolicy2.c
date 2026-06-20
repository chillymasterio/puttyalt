/* puttyalt_passpolicy2.c - Enforce password composition policy.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <ctype.h>
#include <string.h>
typedef struct { int min_len; int need_upper; int need_lower; int need_digit; int need_special; } PassPolicy;
void pp3_default(PassPolicy *p) {
    if (!p) return;
    p->min_len = 8; p->need_upper = 1; p->need_lower = 1; p->need_digit = 1; p->need_special = 0;
}
/* Returns 0 if compliant, else a bitmask of failed requirements. */
int pp3_check(const PassPolicy *p, const char *pw) {
    if (!p || !pw) return -1;
    int up=0, lo=0, dg=0, sp=0, len=(int)strlen(pw);
    for (const char *c = pw; *c; c++) {
        if (isupper((unsigned char)*c)) up=1;
        else if (islower((unsigned char)*c)) lo=1;
        else if (isdigit((unsigned char)*c)) dg=1;
        else sp=1;
    }
    int fail = 0;
    if (len < p->min_len) fail |= 1;
    if (p->need_upper && !up) fail |= 2;
    if (p->need_lower && !lo) fail |= 4;
    if (p->need_digit && !dg) fail |= 8;
    if (p->need_special && !sp) fail |= 16;
    return fail;
}
