/* puttyalt_diffhunk.c - Format unified-diff hunk headers.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stdio.h>
int dh_header(int old_start, int old_count, int new_start, int new_count, char *out, int outlen) {
    if (!out) return -1;
    return snprintf(out, outlen, "@@ -%d,%d +%d,%d @@", old_start, old_count, new_start, new_count);
}
/* Parse "@@ -1,4 +1,6 @@" into the four numbers. Returns 0 on success. */
int dh_parse(const char *line, int *os, int *oc, int *ns, int *nc) {
    if (!line) return -1;
    return (sscanf(line, "@@ -%d,%d +%d,%d @@", os, oc, ns, nc) == 4) ? 0 : -1;
}
char dh_line_kind(const char *line) {
    if (!line || !*line) return ' ';
    if (line[0] == '+') return '+';
    if (line[0] == '-') return '-';
    return ' ';
}
