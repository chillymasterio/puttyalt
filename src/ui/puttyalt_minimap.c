/* puttyalt_minimap.c - Render a scrollbar minimap overview.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
/* Map a document position to a minimap pixel row. */
int mm6_doc_to_map(int doc_line, int doc_total, int map_height) {
    if (doc_total <= 0 || map_height <= 0) return 0;
    return doc_line * map_height / doc_total;
}
int mm6_map_to_doc(int map_row, int map_height, int doc_total) {
    if (map_height <= 0) return 0;
    return map_row * doc_total / map_height;
}
/* Compute the viewport highlight band on the minimap. */
void mm6_viewport(int scroll_top, int viewport_lines, int doc_total, int map_height, int *band_top, int *band_height) {
    if (band_top) *band_top = mm6_doc_to_map(scroll_top, doc_total, map_height);
    if (band_height) {
        int bh = viewport_lines * map_height / (doc_total > 0 ? doc_total : 1);
        *band_height = bh < 1 ? 1 : bh;
    }
}
