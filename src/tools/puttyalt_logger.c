#include <string.h>
#include <stdio.h>
#include <time.h>
#include <stdarg.h>
typedef enum { LOG_DEBUG, LOG_INFO, LOG_WARN, LOG_ERROR } LogLevel;
static FILE *g_logfp = NULL; static LogLevel g_level = LOG_INFO; static int g_console = 1;
void logger_init(const char *path, LogLevel level) {
    g_level = level;
    if (path) g_logfp = fopen(path, "a");
}
void logger_log(LogLevel level, const char *fmt, ...) {
    if (level < g_level) return;
    const char *names[] = {"DEBUG","INFO","WARN","ERROR"};
    char ts[32]; time_t now = time(NULL); strftime(ts, 32, "%H:%M:%S", localtime(&now));
    va_list ap; va_start(ap, fmt);
    char msg[1024]; vsnprintf(msg, 1024, fmt, ap); va_end(ap);
    if (g_logfp) { fprintf(g_logfp, "[%s] %s: %s\n", ts, names[level], msg); fflush(g_logfp); }
    if (g_console) fprintf(stderr, "[%s] %s: %s\n", ts, names[level], msg);
}
void logger_close(void) { if (g_logfp) { fclose(g_logfp); g_logfp = NULL; } }
void logger_set_level(LogLevel level) { g_level = level; }
void logger_set_console(int on) { g_console = on; }
