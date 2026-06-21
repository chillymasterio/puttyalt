/* puttyalt_mimetype.c - Guess MIME type from file extension.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
const char *mt4_from_ext(const char *path) {
    if (!path) return "application/octet-stream";
    const char *dot = strrchr(path, '.');
    if (!dot) return "application/octet-stream";
    const char *ext = dot + 1;
    struct { const char *e; const char *m; } map[] = {
        {"html","text/html"}, {"htm","text/html"}, {"css","text/css"},
        {"js","application/javascript"}, {"json","application/json"},
        {"png","image/png"}, {"jpg","image/jpeg"}, {"jpeg","image/jpeg"},
        {"gif","image/gif"}, {"svg","image/svg+xml"}, {"txt","text/plain"},
        {"pdf","application/pdf"}, {"zip","application/zip"}, {"xml","application/xml"},
    };
    for (unsigned i = 0; i < sizeof(map)/sizeof(map[0]); i++)
        if (strcasecmp(ext, map[i].e) == 0) return map[i].m;
    return "application/octet-stream";
}
