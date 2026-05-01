/*
 * puttyalt_logrotate.c: Session log rotation support.
 *
 * PuTTY's logging can create very large files over time. This module
 * adds automatic log rotation when files exceed a configurable size,
 * keeping N old logs with sequential numbering.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>

#define DEFAULT_MAX_LOG_SIZE  (10 * 1024 * 1024)  /* 10 MB */
#define DEFAULT_MAX_LOG_FILES 5

typedef struct LogRotateConfig {
    long max_size;        /* rotate when log exceeds this (bytes) */
    int max_files;        /* keep this many old logs */
    int compress_old;     /* future: gzip old logs */
} LogRotateConfig;

static void logrotate_defaults(LogRotateConfig *cfg)
{
    cfg->max_size = DEFAULT_MAX_LOG_SIZE;
    cfg->max_files = DEFAULT_MAX_LOG_FILES;
    cfg->compress_old = 0;
}

/*
 * Get size of a file.  Returns -1 on error.
 */
static long file_size(const char *path)
{
    struct stat st;
    if (stat(path, &st) != 0)
        return -1;
    return (long)st.st_size;
}

/*
 * Rotate log files: session.log -> session.log.1,
 * session.log.1 -> session.log.2, etc.
 *
 * Returns 0 on success, -1 on error.
 */
int logrotate_rotate(const char *log_path, const LogRotateConfig *cfg)
{
    LogRotateConfig defaults;
    char old_path[4096], new_path[4096];
    int i;

    if (!cfg) {
        logrotate_defaults(&defaults);
        cfg = &defaults;
    }

    /* Delete the oldest log if it exists */
    snprintf(old_path, sizeof(old_path), "%s.%d", log_path, cfg->max_files);
    remove(old_path);

    /* Shift existing rotated logs */
    for (i = cfg->max_files - 1; i >= 1; i--) {
        snprintf(old_path, sizeof(old_path), "%s.%d", log_path, i);
        snprintf(new_path, sizeof(new_path), "%s.%d", log_path, i + 1);
        rename(old_path, new_path);
    }

    /* Move current log to .1 */
    snprintf(new_path, sizeof(new_path), "%s.1", log_path);
    rename(log_path, new_path);

    return 0;
}

/*
 * Check if log needs rotation based on size threshold.
 * If rotation is needed and performed, returns 1.
 * Returns 0 if no rotation needed, -1 on error.
 */
int logrotate_check(const char *log_path, const LogRotateConfig *cfg)
{
    LogRotateConfig defaults;
    long size;

    if (!cfg) {
        logrotate_defaults(&defaults);
        cfg = &defaults;
    }

    size = file_size(log_path);
    if (size < 0)
        return 0;  /* file doesn't exist yet, nothing to rotate */

    if (size >= cfg->max_size)
        return logrotate_rotate(log_path, cfg) == 0 ? 1 : -1;

    return 0;
}
