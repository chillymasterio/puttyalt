#ifndef PUTTYALT_DEBUGCON_H
#define PUTTYALT_DEBUGCON_H

#define DBG_MAX_LOG     4096
#define DBG_MAX_MSG     512
#define DBG_MAX_WATCH   32
#define DBG_MAX_NAME    64

typedef enum {
    DBG_TRACE = 0,
    DBG_DEBUG,
    DBG_INFO,
    DBG_WARN,
    DBG_ERROR,
    DBG_FATAL
} DbgLevel;

typedef struct {
    DbgLevel level;
    char     module[DBG_MAX_NAME];
    char     message[DBG_MAX_MSG];
    long     timestamp;
    int      line;
} DbgEntry;

typedef struct {
    char  name[DBG_MAX_NAME];
    char  value[256];
    long  last_change;
    int   change_count;
} DbgWatch;

typedef struct {
    DbgEntry  log[DBG_MAX_LOG];
    int       log_count;
    int       log_pos;
    DbgWatch  watches[DBG_MAX_WATCH];
    int       watch_count;
    DbgLevel  min_level;
    char      module_filter[DBG_MAX_NAME];
    int       paused;
    int       enabled;
} DebugConsole;

void dbgcon_init(DebugConsole *dc);
void dbgcon_log(DebugConsole *dc, DbgLevel level, const char *module,
                int line, const char *fmt, ...);
void dbgcon_set_level(DebugConsole *dc, DbgLevel level);
void dbgcon_set_filter(DebugConsole *dc, const char *module);
int  dbgcon_add_watch(DebugConsole *dc, const char *name, const char *initial);
int  dbgcon_update_watch(DebugConsole *dc, const char *name, const char *value);
void dbgcon_clear(DebugConsole *dc);
int  dbgcon_export(const DebugConsole *dc, const char *path);
int  dbgcon_get_filtered(const DebugConsole *dc, int *indices, int max);

#endif
