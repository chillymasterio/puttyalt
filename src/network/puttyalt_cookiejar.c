/* puttyalt_cookiejar.c - Parse and store HTTP cookies.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#define CJ_MAX 32
#define CJ_LEN 64
typedef struct { char name[CJ_LEN]; char value[128]; int used; } Cookie;
typedef struct { Cookie c[CJ_MAX]; } CookieJar;
void cj_init(CookieJar *j) { if (j) for (int i=0;i<CJ_MAX;i++) j->c[i].used=0; }
int cj_set(CookieJar *j, const char *header) {
    if (!j || !header) return -1;
    const char *eq = strchr(header, '=');
    if (!eq) return -1;
    int nlen = (int)(eq - header);
    if (nlen >= CJ_LEN) nlen = CJ_LEN - 1;
    const char *vstart = eq + 1;
    const char *semi = strchr(vstart, ';');
    int vlen = semi ? (int)(semi - vstart) : (int)strlen(vstart);
    if (vlen >= 128) vlen = 127;
    int slot = -1;
    for (int i=0;i<CJ_MAX;i++) {
        if (j->c[i].used && strncmp(j->c[i].name, header, nlen)==0 && j->c[i].name[nlen]==0) { slot=i; break; }
        if (!j->c[i].used && slot<0) slot=i;
    }
    if (slot<0) return -1;
    memcpy(j->c[slot].name, header, nlen); j->c[slot].name[nlen]=0;
    memcpy(j->c[slot].value, vstart, vlen); j->c[slot].value[vlen]=0;
    j->c[slot].used=1;
    return 0;
}
const char *cj_get(const CookieJar *j, const char *name) {
    if (!j||!name) return 0;
    for (int i=0;i<CJ_MAX;i++) if (j->c[i].used && strcmp(j->c[i].name,name)==0) return j->c[i].value;
    return 0;
}
