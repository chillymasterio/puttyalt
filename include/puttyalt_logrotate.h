/*
 * puttyalt_logrotate.h: Automatic session log rotation.
 *
 * Rotates session log files when they exceed a configurable size,
 * keeping a set number of old log files with numeric suffixes.
 */

#ifndef PUTTYALT_LOGROTATE_H
#define PUTTYALT_LOGROTATE_H

#define DEFAULT_MAX_LOG_SIZE   (10 * 1024 * 1024)  /* 10 MB */
#define DEFAULT_MAX_LOG_FILES  5

typedef struct LogRotateConfig {
    long max_size;       /* rotate when file exceeds this size */
    int  max_files;      /* number of rotated files to keep */
    int  compress;       /* future: gzip old logs */
} LogRotateConfig;

/* Check if a log file needs rotation */
int logrotate_check(const char *path, long max_size);

/* Perform the rotation: rename path -> path.1, path.1 -> path.2, etc. */
int logrotate_rotate(const char *path, int max_files);

#endif /* PUTTYALT_LOGROTATE_H */
