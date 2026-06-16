/* puttyalt_mouseev.c - Encode/decode SGR mouse events.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stdio.h>
typedef struct { int button; int col; int row; int pressed; } MouseEvent;
/* Build an SGR mouse report: ESC[<btn;col;rowM/m. Returns length. */
int mev_encode(const MouseEvent *e, char *out, int outlen) {
    if (!e || !out) return -1;
    return snprintf(out, outlen, "\x1b[<%d;%d;%d%c", e->button, e->col, e->row, e->pressed ? 'M' : 'm');
}
/* Parse the body of an SGR mouse sequence (after ESC[<). */
int mev_parse(const char *body, MouseEvent *e) {
    if (!body || !e) return -1;
    int b, c, r;
    char final;
    if (sscanf(body, "%d;%d;%d%c", &b, &c, &r, &final) != 4) return -1;
    e->button = b; e->col = c; e->row = r;
    e->pressed = (final == 'M');
    return 0;
}
int mev_is_wheel(const MouseEvent *e) { return e && (e->button == 64 || e->button == 65); }
