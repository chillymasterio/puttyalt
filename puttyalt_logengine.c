#include "puttyalt_logengine.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>

int logengine_open(LogEngine *le, const char *path, LogFormat fmt)
{
    memset(le, 0, sizeof(*le));
    snprintf(le->path, sizeof(le->path), "%s", path);
    le->format = fmt;
    le->min_level = LOG_LEVEL_INFO;
    le->timestamp_each_line = 1;
    le->flush_on_write = 1;
    le->rotate_size_kb = 10240; /* 10 MB */
    le->rotate_count = 5;

    le->fp = fopen(path, "a");
    if (!le->fp) return -1;

    le->active = 1;
    le->opened_at = (long)time(NULL);

    /* Write header based on format */
    if (fmt == LOG_FORMAT_HTML) {
        fprintf(le->fp, "<html><head><title>PuttyAlt Log</title>"
                "<style>body{font-family:monospace;background:#0d1520;color:#d4dee8}"
                ".warn{color:#ffa726}.error{color:#ef5350}.time{color:#4a9ee0}</style>"
                "</head><body><pre>\n");
    } else if (fmt == LOG_FORMAT_JSON) {
        fprintf(le->fp, "[\n");
    } else if (fmt == LOG_FORMAT_CSV) {
        fprintf(le->fp, "timestamp,level,session,message\n");
    }

    return 0;
}

void logengine_close(LogEngine *le)
{
    if (!le->fp) return;
    if (le->format == LOG_FORMAT_HTML)
        fprintf(le->fp, "</pre></body></html>\n");
    else if (le->format == LOG_FORMAT_JSON)
        fprintf(le->fp, "]\n");
    fclose(le->fp);
    le->fp = NULL;
    le->active = 0;
}

static const char *level_str(LogLevel l)
{
    switch (l) {
    case LOG_LEVEL_DEBUG: return "DEBUG";
    case LOG_LEVEL_INFO:  return "INFO";
    case LOG_LEVEL_WARN:  return "WARN";
    case LOG_LEVEL_ERROR: return "ERROR";
    }
    return "?";
}

int logengine_write(LogEngine *le, LogLevel level, const char *data, int len)
{
    if (!le->active || !le->fp || level < le->min_level) return 0;

    char ts[32] = "";
    if (le->timestamp_each_line) {
        time_t now = time(NULL);
        struct tm *t = localtime(&now);
        strftime(ts, sizeof(ts), "%Y-%m-%d %H:%M:%S", t);
    }

    int written = 0;
    switch (le->format) {
    case LOG_FORMAT_RAW:
        written = (int)fwrite(data, 1, len, le->fp);
        break;
    case LOG_FORMAT_TEXT:
        written = fprintf(le->fp, "[%s] [%s] %.*s\n", ts, level_str(level), len, data);
        break;
    case LOG_FORMAT_HTML:
        written = fprintf(le->fp, "<span class=\"time\">[%s]</span> "
                         "<span class=\"%s\">[%s]</span> %.*s\n",
                         ts, level == LOG_LEVEL_ERROR ? "error" :
                         level == LOG_LEVEL_WARN ? "warn" : "",
                         level_str(level), len, data);
        break;
    case LOG_FORMAT_CSV:
        written = fprintf(le->fp, "%s,%s,%s,\"%.*s\"\n",
                         ts, level_str(level), le->session_name, len, data);
        break;
    case LOG_FORMAT_JSON:
        written = fprintf(le->fp, "{\"ts\":\"%s\",\"level\":\"%s\","
                         "\"session\":\"%s\",\"data\":\"%.*s\"},\n",
                         ts, level_str(level), le->session_name, len, data);
        break;
    }

    le->current_size += written;
    if (le->flush_on_write) fflush(le->fp);

    if (le->rotate_size_kb > 0 && le->current_size >= le->rotate_size_kb * 1024)
        logengine_rotate(le);

    return written;
}

int logengine_writef(LogEngine *le, LogLevel level, const char *fmt, ...)
{
    char buf[4096];
    va_list ap;
    va_start(ap, fmt);
    int len = vsnprintf(buf, sizeof(buf), fmt, ap);
    va_end(ap);
    if (len > 0) return logengine_write(le, level, buf, len);
    return 0;
}

int logengine_rotate(LogEngine *le)
{
    if (!le->fp) return -1;
    fclose(le->fp);

    /* Rotate: .log.4 -> .log.5, etc */
    for (int i = le->rotate_count - 1; i > 0; i--) {
        char old_name[LOG_MAX_PATH + 8], new_name[LOG_MAX_PATH + 8];
        snprintf(old_name, sizeof(old_name), "%s.%d", le->path, i);
        snprintf(new_name, sizeof(new_name), "%s.%d", le->path, i + 1);
        rename(old_name, new_name);
    }

    char rotated[LOG_MAX_PATH + 8];
    snprintf(rotated, sizeof(rotated), "%s.1", le->path);
    rename(le->path, rotated);

    le->fp = fopen(le->path, "w");
    le->current_size = 0;
    return le->fp ? 0 : -1;
}

int logengine_set_session(LogEngine *le, const char *name)
{
    if (!name) return -1;
    snprintf(le->session_name, sizeof(le->session_name), "%s", name);
    return 0;
}

void logengine_flush(LogEngine *le)
{
    if (le->fp) fflush(le->fp);
}
