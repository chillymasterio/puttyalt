/* puttyalt_semver.c - Parse and compare semantic versions.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stdio.h>
typedef struct { int major, minor, patch; } SemVer;
int sv_parse(const char *s, SemVer *out) {
    if (!s || !out) return -1;
    out->major = out->minor = out->patch = 0;
    if (*s == 'v' || *s == 'V') s++;
    if (sscanf(s, "%d.%d.%d", &out->major, &out->minor, &out->patch) < 1) return -1;
    return 0;
}
/* Returns <0, 0, >0 like strcmp. */
int sv_compare(const SemVer *a, const SemVer *b) {
    if (a->major != b->major) return a->major - b->major;
    if (a->minor != b->minor) return a->minor - b->minor;
    return a->patch - b->patch;
}
int sv_satisfies_min(const SemVer *v, const SemVer *min) { return sv_compare(v, min) >= 0; }
