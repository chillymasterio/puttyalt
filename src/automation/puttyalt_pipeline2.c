/* puttyalt_pipeline2.c - Chain command stages with status.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#define PL_MAX 16
#define PL_LEN 64
typedef struct { char cmd[PL_MAX][PL_LEN]; int status[PL_MAX]; int n; int cur; } Pipeline2;
void pl2_init(Pipeline2 *p) { if (p) { p->n = 0; p->cur = 0; } }
int pl2_add(Pipeline2 *p, const char *cmd) {
    if (!p || p->n >= PL_MAX || !cmd) return -1;
    strncpy(p->cmd[p->n], cmd, PL_LEN-1); p->cmd[p->n][PL_LEN-1] = 0;
    p->status[p->n] = -1;
    p->n++;
    return 0;
}
/* Advance: mark current stage with exit code, move to next if ok. */
int pl2_step(Pipeline2 *p, int exit_code) {
    if (!p || p->cur >= p->n) return -1;
    p->status[p->cur] = exit_code;
    if (exit_code != 0) return -1; /* halt on failure */
    p->cur++;
    return p->cur < p->n ? 0 : 1; /* 1 = pipeline complete */
}
const char *pl2_current(const Pipeline2 *p) {
    if (!p || p->cur >= p->n) return 0;
    return p->cmd[p->cur];
}
