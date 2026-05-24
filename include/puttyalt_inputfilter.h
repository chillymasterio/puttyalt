#ifndef PUTTYALT_INPUTFILTER_H
#define PUTTYALT_INPUTFILTER_H

#define IF_MAX_FILTERS  32
#define IF_MAX_NAME     64
#define IF_MAX_PATTERN  256
#define IF_MAX_REPLACE  256

typedef enum {
    IF_INPUT = 0,
    IF_OUTPUT,
    IF_BOTH
} IFDirection;

typedef enum {
    IF_REPLACE = 0,
    IF_STRIP,
    IF_HIGHLIGHT,
    IF_LOG,
    IF_BLOCK,
    IF_TRANSFORM
} IFAction;

typedef struct {
    char        name[IF_MAX_NAME];
    char        pattern[IF_MAX_PATTERN];
    char        replacement[IF_MAX_REPLACE];
    IFDirection direction;
    IFAction    action;
    int         enabled;
    int         case_insensitive;
    long        match_count;
} InputFilter;

typedef struct {
    InputFilter filters[IF_MAX_FILTERS];
    int         count;
    int         global_enabled;
} FilterPipeline;

void ifpipe_init(FilterPipeline *fp);
int  ifpipe_add(FilterPipeline *fp, const char *name, const char *pattern,
                const char *replacement, IFDirection dir, IFAction action);
int  ifpipe_remove(FilterPipeline *fp, int index);
int  ifpipe_enable(FilterPipeline *fp, int index, int enable);
int  ifpipe_process(FilterPipeline *fp, char *buf, int bufsz, IFDirection dir);
int  ifpipe_load(FilterPipeline *fp, const char *path);
int  ifpipe_save(const FilterPipeline *fp, const char *path);
void ifpipe_reset_stats(FilterPipeline *fp);

#endif
