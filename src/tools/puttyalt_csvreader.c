/* puttyalt_csvreader.c - Stream CSV rows from a buffer.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
typedef struct { const char *data; int pos; int len; } CsvReader;
void cr3_init(CsvReader *r, const char *data, int len) { if (r) { r->data = data; r->pos = 0; r->len = len; } }
/* Read the next line into `out`; returns length, or -1 at end. */
int cr3_next_line(CsvReader *r, char *out, int outlen) {
    if (!r || r->pos >= r->len) return -1;
    int o = 0;
    while (r->pos < r->len && r->data[r->pos] != '\n' && o < outlen - 1) {
        if (r->data[r->pos] != '\r') out[o++] = r->data[r->pos];
        r->pos++;
    }
    while (r->pos < r->len && r->data[r->pos] != '\n') r->pos++;
    if (r->pos < r->len) r->pos++; /* skip newline */
    out[o] = 0;
    return o;
}
int cr3_count_rows(const char *data, int len) {
    if (!data) return 0;
    int rows = 0, has_content = 0;
    for (int i = 0; i < len; i++) {
        if (data[i] == '\n') { rows++; has_content = 0; }
        else if (data[i] != '\r') has_content = 1;
    }
    if (has_content) rows++;
    return rows;
}
