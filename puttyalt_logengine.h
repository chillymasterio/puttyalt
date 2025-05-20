#ifndef PUTTYALT_LOGENGINE_H
#define PUTTYALT_LOGENGINE_H

#include <stdio.h>

#define LOG_MAX_PATH 512

typedef enum {
    LOG_FORMAT_RAW,
    LOG_FORMAT_TEXT,
    LOG_FORMAT_HTML,
    LOG_FORMAT_CSV,
    LOG_FORMAT_JSON
} LogFormat;

typedef enum {
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_INFO,
    LOG_LEVEL_WARN,
    LOG_LEVEL_ERROR
} LogLevel;

typedef struct {
    FILE *fp;
    char path[LOG_MAX_PATH];
    LogFormat format;
    LogLevel min_level;
    int timestamp_each_line;
    int flush_on_write;
    int rotate_size_kb;
    int rotate_count;
    int current_size;
    int active;
    long opened_at;
    char session_name[128];
} LogEngine;

int  logengine_open(LogEngine *le, const char *path, LogFormat fmt);
void logengine_close(LogEngine *le);
int  logengine_write(LogEngine *le, LogLevel level, const char *data, int len);
int  logengine_writef(LogEngine *le, LogLevel level, const char *fmt, ...);
int  logengine_rotate(LogEngine *le);
int  logengine_set_session(LogEngine *le, const char *name);
void logengine_flush(LogEngine *le);

#endif
