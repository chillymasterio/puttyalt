/* puttyalt_methodtable.c - HTTP method classification.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
int mth_is_safe(const char *method) {
    return method && (!strcmp(method,"GET") || !strcmp(method,"HEAD") || !strcmp(method,"OPTIONS"));
}
int mth_is_idempotent(const char *method) {
    return method && (mth_is_safe(method) || !strcmp(method,"PUT") || !strcmp(method,"DELETE"));
}
int mth_has_body(const char *method) {
    return method && (!strcmp(method,"POST") || !strcmp(method,"PUT") || !strcmp(method,"PATCH"));
}
int mth_is_valid(const char *method) {
    static const char *valid[] = {"GET","HEAD","POST","PUT","DELETE","PATCH","OPTIONS","TRACE","CONNECT"};
    if (!method) return 0;
    for (unsigned i = 0; i < sizeof(valid)/sizeof(valid[0]); i++) if (!strcmp(method, valid[i])) return 1;
    return 0;
}
