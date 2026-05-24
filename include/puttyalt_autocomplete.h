/*
 * puttyalt_autocomplete.h: Command auto-completion.
 *
 * Provides tab-completion suggestions based on command history
 * and a built-in dictionary of common Unix/Linux commands.
 */

#ifndef PUTTYALT_AUTOCOMPLETE_H
#define PUTTYALT_AUTOCOMPLETE_H

#define AC_MAX_SUGGESTIONS   20
#define AC_MAX_WORD_LEN      128
#define AC_DICT_SIZE         256

typedef struct AutoCompleteSuggestion {
    char text[AC_MAX_WORD_LEN];
    int frequency;            /* how often this was used */
    int from_history;         /* 1 if from command history */
} AutoCompleteSuggestion;

typedef struct AutoComplete {
    AutoCompleteSuggestion suggestions[AC_MAX_SUGGESTIONS];
    int num_suggestions;
    int selected;             /* currently highlighted suggestion */
    char prefix[AC_MAX_WORD_LEN];
    int active;               /* popup visible? */
    char dictionary[AC_DICT_SIZE][AC_MAX_WORD_LEN];
    int dict_count;
} AutoComplete;

void ac_init(AutoComplete *ac);
void ac_load_defaults(AutoComplete *ac);
int  ac_update(AutoComplete *ac, const char *current_word);
const char *ac_get_selected(const AutoComplete *ac);
int  ac_next(AutoComplete *ac);
int  ac_prev(AutoComplete *ac);
void ac_dismiss(AutoComplete *ac);
int  ac_add_to_dict(AutoComplete *ac, const char *word);

#endif /* PUTTYALT_AUTOCOMPLETE_H */
