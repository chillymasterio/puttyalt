/* puttyalt_oscparse.c - Parse OSC (operating system command) strings.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stdlib.h>
#include <string.h>
typedef struct { int code; char text[256]; } OscSeq;
/* Parse "0;title" style OSC body (after ESC]). Returns 0 on success. */
int osc_parse(const char *body, OscSeq *out) {
    if (!body || !out) return -1;
    out->code = atoi(body);
    const char *semi = strchr(body, ';');
    if (semi) {
        strncpy(out->text, semi + 1, sizeof(out->text) - 1);
        out->text[sizeof(out->text)-1] = 0;
    } else out->text[0] = 0;
    return 0;
}
int osc_is_title(const OscSeq *s) { return s && (s->code == 0 || s->code == 2); }
int osc_is_hyperlink(const OscSeq *s) { return s && s->code == 8; }
int osc_is_clipboard(const OscSeq *s) { return s && s->code == 52; }
