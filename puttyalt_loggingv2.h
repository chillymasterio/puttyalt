/*
 * puttyalt_loggingv2.h: Structured session logging with rotation.
 *
 * Provides JSON-formatted log output with severity levels,
 * automatic file rotation, and configurable retention.
 */

#ifndef PUTTYALT_LOGGINGV2_H
#define PUTTYALT_LOGGINGV2_H

#include <stdio.h>
#include <time.h>

#define LOG_MAX_MSG      4096
#define LOG_MAX_PATH     512
#define LOG_DEFAULT_SIZE (10 * 1024 * 1024)  /* 10 MB */
#define LOG_DEFAULT_KEEP 5

typedef enum {
    LOG_TRACE = 0,
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARN,
    LOG_ERROR,
    LOG_FATAL
} LogLevel;

typedef enum {
    LOG_FMT_TEXT = 0,
    LOG_FMT_JSON,
    LOG_FMT_CSV
} LogFormat;

typedef struct LogConfig {
    char path[LOG_MAX_PATH];
    LogLevel min_level;
    LogFormat format;
    unsigned long max_size;   /* bytes before rotation */
    int max_files;            /* rotated files to keep */
    int log_timestamps;
    int log_session_name;
    int flush_immediate;
} LogConfig;

typedef struct Logger {
    LogConfig config;
    FILE *fp;
    unsigned long bytes_written;
    int file_index;
    time_t opened_at;
} Logger;

int  logger_open(Logger *log, const LogConfig *config);
void logger_close(Logger *log);
void logger_write(Logger *log, LogLevel level, const char *session,
                  const char *fmt, ...);
void logger_rotate(Logger *log);
const char *log_level_str(LogLevel level);

#endif /* PUTTYALT_LOGGINGV2_H */
