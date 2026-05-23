#include <stdio.h>
#include "puttyalt_autocomplete_v2.h"
#include <string.h>
#include <ctype.h>

void acv2_init(AutoCompleteV2 *ac)
{
    memset(ac, 0, sizeof(*ac));
    ac->min_chars = 2;
    ac->learn_commands = 1;
    ac->suggest_paths = 1;
    ac->suggest_hosts = 1;
}

void acv2_learn(AutoCompleteV2 *ac, const char *cmd)
{
    if (!cmd || !cmd[0]) return;
    /* check if already in history */
    for (int i = 0; i < ac->history_count; i++) {
        if (strcmp(ac->history[i], cmd) == 0) {
            /* move to front (most recent) */
            char tmp[256];
            snprintf(tmp, sizeof(tmp), "%s", ac->history[i]);
            memmove(&ac->history[1], &ac->history[0], i * 256);
            snprintf(ac->history[0], 256, "%s", tmp);
            return;
        }
    }
    /* add to front */
    if (ac->history_count < AC_MAX_HISTORY) ac->history_count++;
    memmove(&ac->history[1], &ac->history[0], (ac->history_count - 1) * 256);
    snprintf(ac->history[0], 256, "%s", cmd);
}

static int prefix_match(const char *str, const char *prefix, int case_insensitive)
{
    int plen = (int)strlen(prefix);
    for (int i = 0; i < plen; i++) {
        char a = case_insensitive ? tolower((unsigned char)str[i]) : str[i];
        char b = case_insensitive ? tolower((unsigned char)prefix[i]) : prefix[i];
        if (a != b) return 0;
    }
    return 1;
}

int acv2_suggest(AutoCompleteV2 *ac, const char *prefix)
{
    ac->suggestion_count = 0;
    ac->selected = 0;
    if (!prefix || (int)strlen(prefix) < ac->min_chars) return 0;

    for (int i = 0; i < ac->history_count && ac->suggestion_count < AC_MAX_SUGGESTIONS; i++) {
        if (prefix_match(ac->history[i], prefix, 1) && strcmp(ac->history[i], prefix) != 0) {
            ACSuggestion *s = &ac->suggestions[ac->suggestion_count++];
            snprintf(s->text, sizeof(s->text), "%s", ac->history[i]);
            s->recency_score = ac->history_count - i;
            snprintf(s->category, sizeof(s->category), "history");
        }
    }
    ac->active = ac->suggestion_count > 0;
    return ac->suggestion_count;
}

void acv2_select_next(AutoCompleteV2 *ac)
{
    if (ac->suggestion_count > 0) ac->selected = (ac->selected + 1) % ac->suggestion_count;
}

void acv2_select_prev(AutoCompleteV2 *ac)
{
    if (ac->suggestion_count > 0) ac->selected = (ac->selected - 1 + ac->suggestion_count) % ac->suggestion_count;
}

const char *acv2_get_selected(AutoCompleteV2 *ac)
{
    return (ac->active && ac->selected < ac->suggestion_count) ? ac->suggestions[ac->selected].text : NULL;
}

void acv2_accept(AutoCompleteV2 *ac) { ac->active = 0; }
void acv2_dismiss(AutoCompleteV2 *ac) { ac->active = 0; ac->suggestion_count = 0; }
