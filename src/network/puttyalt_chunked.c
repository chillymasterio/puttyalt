/* puttyalt_chunked.c - Decode HTTP chunked transfer encoding.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
/* Parse a chunk size line (hex). Returns size, or -1 on error. */
long ck_chunk_size(const char *line) {
    if (!line) return -1;
    long v = 0; int seen = 0;
    for (const char *p = line; *p && *p != '\r' && *p != '\n' && *p != ';'; p++) {
        int d;
        if (*p>='0'&&*p<='9') d=*p-'0';
        else if (*p>='a'&&*p<='f') d=*p-'a'+10;
        else if (*p>='A'&&*p<='F') d=*p-'A'+10;
        else break;
        v = v*16 + d; seen = 1;
    }
    return seen ? v : -1;
}
int ck_is_last(const char *line) { return ck_chunk_size(line) == 0; }
