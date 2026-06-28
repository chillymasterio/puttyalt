/* puttyalt_sandboxpolicy.c - Evaluate plugin sandbox capability grants.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
enum { CAP_NETWORK=1, CAP_FILE_READ=2, CAP_FILE_WRITE=4, CAP_EXEC=8, CAP_CLIPBOARD=16 };
typedef struct { int granted; } SandboxPolicy;
void sbp_init(SandboxPolicy *p) { if (p) p->granted = 0; }
int sbp_grant(SandboxPolicy *p, const char *cap) {
    if (!p || !cap) return -1;
    if (!strcmp(cap, "network")) p->granted |= CAP_NETWORK;
    else if (!strcmp(cap, "file_read")) p->granted |= CAP_FILE_READ;
    else if (!strcmp(cap, "file_write")) p->granted |= CAP_FILE_WRITE;
    else if (!strcmp(cap, "exec")) p->granted |= CAP_EXEC;
    else if (!strcmp(cap, "clipboard")) p->granted |= CAP_CLIPBOARD;
    else return -1;
    return 0;
}
int sbp_allowed(const SandboxPolicy *p, int cap) { return p && (p->granted & cap) == cap; }
int sbp_is_dangerous(const SandboxPolicy *p) {
    return p && (p->granted & (CAP_EXEC | CAP_FILE_WRITE)) != 0;
}
