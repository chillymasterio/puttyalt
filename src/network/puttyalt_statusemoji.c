/* puttyalt_statusemoji.c - Map HTTP status to category labels.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
const char *se4_symbol(int code) {
    if (code >= 200 && code < 300) return "[OK]";
    if (code >= 300 && code < 400) return "[->]";
    if (code >= 400 && code < 500) return "[!]";
    if (code >= 500) return "[XX]";
    return "[..]";
}
const char *se4_category(int code) {
    switch (code / 100) {
        case 1: return "informational";
        case 2: return "success";
        case 3: return "redirect";
        case 4: return "client error";
        case 5: return "server error";
        default: return "unknown";
    }
}
