/* puttyalt_ftpcode.c - Classify FTP reply codes.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
int ftp_is_positive(int code) { return code >= 200 && code < 400; }
int ftp_is_intermediate(int code) { return code >= 300 && code < 400; }
int ftp_is_error(int code) { return code >= 400 && code < 600; }
int ftp_is_complete(const char *line) {
    /* multiline replies use "123-"; final line uses "123 " */
    if (!line) return 0;
    return line[0] && line[1] && line[2] && line[3] == ' ';
}
const char *ftp_category(int code) {
    switch (code / 100) {
        case 1: return "positive preliminary";
        case 2: return "positive completion";
        case 3: return "positive intermediate";
        case 4: return "transient negative";
        case 5: return "permanent negative";
        default: return "unknown";
    }
}
