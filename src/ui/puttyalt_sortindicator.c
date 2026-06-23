/* puttyalt_sortindicator.c - Manage table sort column/direction.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
typedef struct { int column; int ascending; } SortState;
void si2_init(SortState *s) { if (s) { s->column = -1; s->ascending = 1; } }
/* Click a column header: toggle direction or switch column. */
void si2_click(SortState *s, int column) {
    if (!s) return;
    if (s->column == column) s->ascending = !s->ascending;
    else { s->column = column; s->ascending = 1; }
}
const char *si2_arrow(const SortState *s, int column) {
    if (!s || s->column != column) return "";
    return s->ascending ? " ^" : " v";
}
