/* puttyalt_wraptext2.c - Hard-wrap text preserving word boundaries.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
/* Count how many display lines text needs at a given width. */
int wt3_line_count(const char *s, int width) {
    if (!s || width <= 0) return 0;
    int lines = 1, col = 0, last_space_col = -1;
    for (const char *p = s; *p; p++) {
        if (*p == '\n') { lines++; col = 0; last_space_col = -1; continue; }
        col++;
        if (*p == ' ') last_space_col = col;
        if (col > width) {
            lines++;
            col = (last_space_col > 0) ? col - last_space_col : 1;
            last_space_col = -1;
        }
    }
    return lines;
}
