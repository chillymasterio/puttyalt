/* puttyalt_wordcount.c - Count words, lines, characters.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <ctype.h>
typedef struct { int chars; int words; int lines; } WordCount;
void wc3_count(const char *s, WordCount *out) {
    if (!s || !out) return;
    out->chars = out->words = out->lines = 0;
    int in_word = 0;
    for (const char *p = s; *p; p++) {
        out->chars++;
        if (*p == '\n') out->lines++;
        if (isspace((unsigned char)*p)) in_word = 0;
        else if (!in_word) { in_word = 1; out->words++; }
    }
    if (out->chars > 0 && s[out->chars-1] != '\n') out->lines++;
}
