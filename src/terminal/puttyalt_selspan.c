/* puttyalt_selspan.c - Compute text selection spans.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
typedef struct { int start_row, start_col, end_row, end_col; } SelSpan;
/* Normalize so start is before end in reading order. */
SelSpan selspan_normalize(SelSpan s) {
    if (s.start_row > s.end_row || (s.start_row == s.end_row && s.start_col > s.end_col)) {
        SelSpan r; r.start_row = s.end_row; r.start_col = s.end_col;
        r.end_row = s.start_row; r.end_col = s.start_col; return r;
    }
    return s;
}
/* Is a cell within the selection (linewise reading order)? */
int selspan_contains(SelSpan s, int row, int col) {
    s = selspan_normalize(s);
    if (row < s.start_row || row > s.end_row) return 0;
    if (row == s.start_row && col < s.start_col) return 0;
    if (row == s.end_row && col >= s.end_col) return 0;
    return 1;
}
int selspan_is_empty(SelSpan s) { return s.start_row == s.end_row && s.start_col == s.end_col; }
