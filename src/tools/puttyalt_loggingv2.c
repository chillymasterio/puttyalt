/*
 * puttyalt_loggingv2.c: Structured session logging implementation.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>
#include "puttyalt_loggingv2.h"

static const char *level_names[] = {
    "TRACE", "DEBUG", "INFO", "WARN", "ERROR", "FATAL"
};

const char *log_level_str(LogLevel level)
{
    if (level >= LOG_TRACE && level <= LOG_FATAL)
        return level_names[level];
    return "UNKNOWN";
}

int logger_open(Logger *log, const LogConfig *config)
{
    memset(log, 0, sizeof(*log));
    log->config = *config;

    log->fp = fopen(config->path, "a");
    if (!log->fp)
        return -1;

    log->opened_at = time(NULL);
    return 0;
}

void logger_close(Logger *log)
{
    if (log->fp) {
        fflush(log->fp);
        fclose(log->fp);
        log->fp = NULL;
    }
}

void logger_rotate(Logger *log)
{
    if (!log->fp)
        return;

    fclose(log->fp);
    log->fp = NULL;

    /* Rename existing files: log.4 -> log.5, log.3 -> log.4, etc. */
    char old_path[LOG_MAX_PATH + 16];
    char new_path[LOG_MAX_PATH + 16];

    for (int i = log->config.max_files - 1; i >= 1; i--) {
        snprintf(old_path, sizeof(old_path), "%s.%d", log->config.path, i);
        snprintf(new_path, sizeof(new_path), "%s.%d", log->config.path, i + 1);
        rename(old_path, new_path);
    }

    snprintf(new_path, sizeof(new_path), "%s.1", log->config.path);
    rename(log->config.path, new_path);

    log->fp = fopen(log->config.path, "w");
    log->bytes_written = 0;
    log->file_index++;
}

void logger_write(Logger *log, LogLevel level, const char *session,
                  const char *fmt, ...)
{
    if (!log->fp)
        return;
    if (level < log->config.min_level)
        return;

    /* Check rotation */
    if (log->config.max_size > 0 &&
        log->bytes_written >= log->config.max_size) {
        logger_rotate(log);
        if (!log->fp)
            return;
    }

    char msg[LOG_MAX_MSG];
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(msg, sizeof(msg), fmt, ap);
    va_end(ap);

    char timestamp[64] = "";
    if (log->config.log_timestamps) {
        time_t now = time(NULL);
        struct tm *tm = localtime(&now);
        strftime(timestamp, sizeof(timestamp), "%Y-%m-%dT%H:%M:%S", tm);
    }

    int written = 0;
    switch (log->config.format) {
    case LOG_FMT_JSON:
        written = fprintf(log->fp,
            "{\"time\":\"%s\",\"level\":\"%s\",\"session\":\"%s\","
            "\"msg\":\"%s\"}\n",
            timestamp, log_level_str(level),
            session ? session : "", msg);
        break;
    case LOG_FMT_CSV:
        written = fprintf(log->fp, "%s,%s,%s,\"%s\"\n",
            timestamp, log_level_str(level),
            session ? session : "", msg);
        break;
    default: /* LOG_FMT_TEXT */
        written = fprintf(log->fp, "[%s] %s",
            log_level_str(level), "");
        if (log->config.log_timestamps)
            written += fprintf(log->fp, "%s ", timestamp);
        if (log->config.log_session_name && session)
            written += fprintf(log->fp, "[%s] ", session);
        written += fprintf(log->fp, "%s\n", msg);
        break;
    }

    if (written > 0)
        log->bytes_written += written;

    if (log->config.flush_immediate)
        fflush(log->fp);
}
