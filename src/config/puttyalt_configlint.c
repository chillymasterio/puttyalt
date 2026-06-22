/* puttyalt_configlint.c - Lint config files for common mistakes.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#include <ctype.h>
enum { CL_OK, CL_EMPTY_VALUE, CL_NO_EQUALS, CL_TRAILING_SPACE, CL_DUP_KEY };
/* Lint a single "key=value" line. Returns a CL_* code. */
int cl3_line(const char *line) {
    if (!line) return CL_OK;
    while (*line == ' ' || *line == '\t') line++;
    if (*line == 0 || *line == '#') return CL_OK;
    const char *eq = strchr(line, '=');
    if (!eq) return CL_NO_EQUALS;
    if (eq[1] == 0 || eq[1] == '\n') return CL_EMPTY_VALUE;
    int len = (int)strlen(line);
    if (len > 0 && (line[len-1] == ' ' || line[len-1] == '\t')) return CL_TRAILING_SPACE;
    return CL_OK;
}
const char *cl3_message(int code) {
    switch (code) {
        case CL_EMPTY_VALUE: return "empty value";
        case CL_NO_EQUALS: return "missing '='";
        case CL_TRAILING_SPACE: return "trailing whitespace";
        case CL_DUP_KEY: return "duplicate key";
        default: return "ok";
    }
}
