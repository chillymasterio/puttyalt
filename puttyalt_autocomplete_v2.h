#ifndef PUTTYALT_AUTOCOMPLETE_V2_H
#define PUTTYALT_AUTOCOMPLETE_V2_H

#define AC_MAX_SUGGESTIONS 32
#define AC_MAX_HISTORY 512

typedef struct {
    char text[256];
    int  frequency;
    int  recency_score;
    char category[32]; /* command, path, host, user */
} ACSuggestion;

typedef struct {
    char history[AC_MAX_HISTORY][256];
    int  history_count;
    ACSuggestion suggestions[AC_MAX_SUGGESTIONS];
    int  suggestion_count;
    int  selected;
    int  active;
    int  min_chars;   /* min chars before showing suggestions */
    int  learn_commands; /* learn from typed commands */
    int  suggest_paths;
    int  suggest_hosts;
} AutoCompleteV2;

void acv2_init(AutoCompleteV2 *ac);
void acv2_learn(AutoCompleteV2 *ac, const char *cmd);
int  acv2_suggest(AutoCompleteV2 *ac, const char *prefix);
void acv2_select_next(AutoCompleteV2 *ac);
void acv2_select_prev(AutoCompleteV2 *ac);
const char *acv2_get_selected(AutoCompleteV2 *ac);
void acv2_accept(AutoCompleteV2 *ac);
void acv2_dismiss(AutoCompleteV2 *ac);

#endif
