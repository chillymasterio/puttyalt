/* puttyalt_cursorreport.c - Parse cursor position report (CPR).
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stdio.h>
/* Parse a "ESC[row;colR" CPR response body (after ESC[). */
int crp_parse(const char *body, int *row, int *col) {
    if (!body || !row || !col) return -1;
    return (sscanf(body, "%d;%dR", row, col) == 2) ? 0 : -1;
}
/* Build a CPR request. */
int crp_request(char *out, int outlen) {
    if (!out || outlen < 5) return -1;
    out[0]='\x1b'; out[1]='['; out[2]='6'; out[3]='n'; out[4]=0;
    return 4;
}
/* Build a cursor-position report response. */
int crp_response(int row, int col, char *out, int outlen) {
    return snprintf(out, outlen, "\x1b[%d;%dR", row, col);
}
