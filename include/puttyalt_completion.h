#ifndef PUTTYALT_COMPLETION_H
#define PUTTYALT_COMPLETION_H

#define COMP_MAX_WORDS      2048
#define COMP_MAX_WORD       128
#define COMP_MAX_RESULTS    64
#define COMP_MAX_HISTORY    512

typedef enum {
    COMP_SRC_HISTORY = 0,
    COMP_SRC_PATH,
    COMP_SRC_HOSTNAME,
    COMP_SRC_COMMAND,
    COMP_SRC_CUSTOM
} CompSource;

typedef struct {
    char word[COMP_MAX_WORD];
    int  frequency;
    CompSource source;
} CompWord;

typedef struct {
    CompWord  words[COMP_MAX_WORDS];
    int       word_count;
    char      history[COMP_MAX_HISTORY][COMP_MAX_WORD];
    int       hist_count;
    CompWord  results[COMP_MAX_RESULTS];
    int       result_count;
    int       selected;
    int       enabled;
    int       min_prefix;    /* minimum chars before completion */
} CompEngine;

void comp_init(CompEngine *ce);
int  comp_add_word(CompEngine *ce, const char *word, CompSource src);
int  comp_add_history(CompEngine *ce, const char *cmd);
int  comp_complete(CompEngine *ce, const char *prefix);
const char *comp_get_selected(const CompEngine *ce);
void comp_select_next(CompEngine *ce);
void comp_select_prev(CompEngine *ce);
void comp_dismiss(CompEngine *ce);
int  comp_load_wordlist(CompEngine *ce, const char *path, CompSource src);
void comp_boost(CompEngine *ce, const char *word);

#endif
