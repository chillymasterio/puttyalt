/* puttyalt_permcheck.c - Evaluate access-control permission bits.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
enum { PERM_READ = 1, PERM_WRITE = 2, PERM_EXEC = 4, PERM_ADMIN = 8 };
typedef struct { int granted; } PermSet;
void pc3_init(PermSet *p, int initial) { if (p) p->granted = initial; }
void pc3_grant(PermSet *p, int bits) { if (p) p->granted |= bits; }
void pc3_revoke(PermSet *p, int bits) { if (p) p->granted &= ~bits; }
int pc3_can(const PermSet *p, int bits) { return p && (p->granted & bits) == bits; }
/* Render rwx-style string into a 5-char buffer. */
int pc3_string(const PermSet *p, char *out, int outlen) {
    if (!p || !out || outlen < 5) return -1;
    out[0] = (p->granted & PERM_READ)  ? 'r' : '-';
    out[1] = (p->granted & PERM_WRITE) ? 'w' : '-';
    out[2] = (p->granted & PERM_EXEC)  ? 'x' : '-';
    out[3] = (p->granted & PERM_ADMIN) ? 'A' : '-';
    out[4] = 0;
    return 4;
}
