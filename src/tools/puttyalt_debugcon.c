#include "puttyalt_debugcon.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>

void dbgcon_init(DebugConsole *dc)
{
    memset(dc, 0, sizeof(*dc));
    dc->enabled = 1;
    dc->min_level = DBG_DEBUG;
}

void dbgcon_log(DebugConsole *dc, DbgLevel level, const char *module,
                int line, const char *fmt, ...)
{
    if (!dc->enabled || dc->paused || level < dc->min_level) return;

    DbgEntry *e = &dc->log[dc->log_pos];
    e->level = level;
    e->line = line;
    e->timestamp = (long)time(NULL);
    snprintf(e->module, DBG_MAX_NAME, "%s", module ? module : "");

    va_list ap;
    va_start(ap, fmt);
    vsnprintf(e->message, DBG_MAX_MSG, fmt, ap);
    va_end(ap);

    dc->log_pos = (dc->log_pos + 1) % DBG_MAX_LOG;
    if (dc->log_count < DBG_MAX_LOG) dc->log_count++;
}

void dbgcon_set_level(DebugConsole *dc, DbgLevel level)
{
    dc->min_level = level;
}

void dbgcon_set_filter(DebugConsole *dc, const char *module)
{
    if (module) snprintf(dc->module_filter, DBG_MAX_NAME, "%s", module);
    else dc->module_filter[0] = '\0';
}

int dbgcon_add_watch(DebugConsole *dc, const char *name, const char *initial)
{
    if (dc->watch_count >= DBG_MAX_WATCH) return -1;
    DbgWatch *w = &dc->watches[dc->watch_count];
    snprintf(w->name, DBG_MAX_NAME, "%s", name);
    if (initial) snprintf(w->value, sizeof(w->value), "%s", initial);
    w->last_change = (long)time(NULL);
    return dc->watch_count++;
}

int dbgcon_update_watch(DebugConsole *dc, const char *name, const char *value)
{
    for (int i = 0; i < dc->watch_count; i++) {
        if (strcmp(dc->watches[i].name, name) == 0) {
            if (strcmp(dc->watches[i].value, value) != 0) {
                snprintf(dc->watches[i].value, sizeof(dc->watches[i].value),
                         "%s", value);
                dc->watches[i].last_change = (long)time(NULL);
                dc->watches[i].change_count++;
            }
            return i;
        }
    }
    return -1;
}

void dbgcon_clear(DebugConsole *dc)
{
    dc->log_count = 0;
    dc->log_pos = 0;
}

int dbgcon_export(const DebugConsole *dc, const char *path)
{
    FILE *f = fopen(path, "w");
    if (!f) return -1;
    static const char *level_names[] = {
        "TRACE", "DEBUG", "INFO", "WARN", "ERROR", "FATAL"
    };
    int start = dc->log_count >= DBG_MAX_LOG ?
                dc->log_pos : 0;
    int count = dc->log_count < DBG_MAX_LOG ?
                dc->log_count : DBG_MAX_LOG;
    for (int i = 0; i < count; i++) {
        int idx = (start + i) % DBG_MAX_LOG;
        const DbgEntry *e = &dc->log[idx];
        const char *lvl = e->level <= DBG_FATAL ? level_names[e->level] : "?";
        fprintf(f, "[%s] %s:%d %s\n", lvl, e->module, e->line, e->message);
    }
    fclose(f);
    return count;
}

int dbgcon_get_filtered(const DebugConsole *dc, int *indices, int max)
{
    int n = 0;
    int start = dc->log_count >= DBG_MAX_LOG ? dc->log_pos : 0;
    int count = dc->log_count < DBG_MAX_LOG ? dc->log_count : DBG_MAX_LOG;
    for (int i = 0; i < count && n < max; i++) {
        int idx = (start + i) % DBG_MAX_LOG;
        const DbgEntry *e = &dc->log[idx];
        if (e->level < dc->min_level) continue;
        if (dc->module_filter[0] && strcmp(e->module, dc->module_filter) != 0)
            continue;
        indices[n++] = idx;
    }
    return n;
}
