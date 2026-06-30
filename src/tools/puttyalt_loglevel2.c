/* puttyalt_loglevel2.c - Log level parsing and filtering.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
enum { LOG_TRACE, LOG_DEBUG, LOG_INFO, LOG_WARN, LOG_ERROR, LOG_FATAL };
int ll4_parse(const char *name) {
    if (!name) return LOG_INFO;
    if (!strcasecmp(name, "trace")) return LOG_TRACE;
    if (!strcasecmp(name, "debug")) return LOG_DEBUG;
    if (!strcasecmp(name, "info")) return LOG_INFO;
    if (!strcasecmp(name, "warn") || !strcasecmp(name, "warning")) return LOG_WARN;
    if (!strcasecmp(name, "error")) return LOG_ERROR;
    if (!strcasecmp(name, "fatal")) return LOG_FATAL;
    return LOG_INFO;
}
const char *ll4_name(int level) {
    static const char *names[] = {"TRACE","DEBUG","INFO","WARN","ERROR","FATAL"};
    return (level >= 0 && level <= LOG_FATAL) ? names[level] : "INFO";
}
int ll4_enabled(int level, int threshold) { return level >= threshold; }
