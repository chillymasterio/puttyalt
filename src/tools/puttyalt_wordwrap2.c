/* puttyalt_wordwrap2.c - Wrap text to a column width.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */

/* Greedy word-wrap: copies `s` into `out`, inserting newlines so no line
   exceeds `width` columns. Breaks at the last space when possible, otherwise
   hard-breaks an over-long word. Returns output length, or -1 on bad args. */
int ww2_wrap(const char *s, int width, char *out, int outlen) {
    if (!s || !out || width <= 0 || outlen <= 0) return -1;
    int o = 0, col = 0, last_space = -1;
    while (*s && o < outlen - 1) {
        char c = *s++;
        if (c == '\n') {
            out[o++] = '\n';
            col = 0; last_space = -1;
            continue;
        }
        out[o++] = c;
        col++;
        if (c == ' ') last_space = o - 1;
        if (col > width) {
            if (last_space >= 0) {
                /* Convert the remembered space into a line break. */
                out[last_space] = '\n';
                col = o - last_space - 1;
                last_space = -1;
            } else if (o < outlen - 1) {
                /* No space to break on: hard-break before the current char. */
                char held = out[o - 1];
                out[o - 1] = '\n';
                out[o++] = held;
                col = 1;
            }
        }
    }
    out[o] = 0;
    return o;
}
