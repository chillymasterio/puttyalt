/* puttyalt_backuprotate.c - Rotate numbered backup files.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stdio.h>
#include <string.h>
/* Compute the backup filename for generation `gen`. */
int br3_name(const char *base, int gen, char *out, int outlen) {
    if (!base || !out) return -1;
    if (gen == 0) return snprintf(out, outlen, "%s", base);
    return snprintf(out, outlen, "%s.%d", base, gen);
}
/* Given a max retention, return how many backups to keep. */
int br3_should_keep(int gen, int max_keep) { return gen <= max_keep; }
/* Compute next rotation: returns oldest gen to delete, or -1. */
int br3_oldest_to_drop(int current_count, int max_keep) {
    return current_count > max_keep ? max_keep + 1 : -1;
}
