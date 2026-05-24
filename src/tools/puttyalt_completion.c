#include "puttyalt_completion.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void comp_init(CompEngine *ce)
{
    memset(ce, 0, sizeof(*ce));
    ce->enabled = 1;
    ce->min_prefix = 2;
    ce->selected = -1;
}

int comp_add_word(CompEngine *ce, const char *word, CompSource src)
{
    if (!word || !*word || ce->word_count >= COMP_MAX_WORDS) return -1;
    /* Check for duplicate */
    for (int i = 0; i < ce->word_count; i++) {
        if (strcmp(ce->words[i].word, word) == 0) {
            ce->words[i].frequency++;
            return i;
        }
    }
    CompWord *w = &ce->words[ce->word_count];
    snprintf(w->word, COMP_MAX_WORD, "%s", word);
    w->frequency = 1;
    w->source = src;
    return ce->word_count++;
}

int comp_add_history(CompEngine *ce, const char *cmd)
{
    if (!cmd || !*cmd) return -1;
    /* Avoid duplicates at end */
    if (ce->hist_count > 0 &&
        strcmp(ce->history[ce->hist_count - 1], cmd) == 0)
        return 0;
    if (ce->hist_count >= COMP_MAX_HISTORY) {
        memmove(ce->history[0], ce->history[1],
                (COMP_MAX_HISTORY - 1) * COMP_MAX_WORD);
        ce->hist_count--;
    }
    snprintf(ce->history[ce->hist_count], COMP_MAX_WORD, "%s", cmd);
    ce->hist_count++;
    /* Also add words from the command */
    char buf[COMP_MAX_WORD];
    snprintf(buf, sizeof(buf), "%s", cmd);
    char *tok = strtok(buf, " \t");
    while (tok) {
        if (strlen(tok) >= (size_t)ce->min_prefix)
            comp_add_word(ce, tok, COMP_SRC_HISTORY);
        tok = strtok(NULL, " \t");
    }
    return 0;
}

static int comp_cmp(const void *a, const void *b)
{
    const CompWord *wa = (const CompWord *)a;
    const CompWord *wb = (const CompWord *)b;
    return wb->frequency - wa->frequency;
}

int comp_complete(CompEngine *ce, const char *prefix)
{
    ce->result_count = 0;
    ce->selected = -1;
    if (!ce->enabled || !prefix) return 0;
    size_t plen = strlen(prefix);
    if (plen < (size_t)ce->min_prefix) return 0;

    for (int i = 0; i < ce->word_count && ce->result_count < COMP_MAX_RESULTS; i++) {
        if (strncmp(ce->words[i].word, prefix, plen) == 0 &&
            strlen(ce->words[i].word) > plen) {
            ce->results[ce->result_count++] = ce->words[i];
        }
    }
    if (ce->result_count > 1)
        qsort(ce->results, ce->result_count, sizeof(CompWord), comp_cmp);
    if (ce->result_count > 0) ce->selected = 0;
    return ce->result_count;
}

const char *comp_get_selected(const CompEngine *ce)
{
    if (ce->selected < 0 || ce->selected >= ce->result_count) return NULL;
    return ce->results[ce->selected].word;
}

void comp_select_next(CompEngine *ce)
{
    if (ce->result_count == 0) return;
    ce->selected = (ce->selected + 1) % ce->result_count;
}

void comp_select_prev(CompEngine *ce)
{
    if (ce->result_count == 0) return;
    ce->selected = (ce->selected - 1 + ce->result_count) % ce->result_count;
}

void comp_dismiss(CompEngine *ce)
{
    ce->result_count = 0;
    ce->selected = -1;
}

int comp_load_wordlist(CompEngine *ce, const char *path, CompSource src)
{
    FILE *f = fopen(path, "r");
    char line[COMP_MAX_WORD];
    int added = 0;
    if (!f) return -1;
    while (fgets(line, sizeof(line), f)) {
        size_t len = strlen(line);
        while (len > 0 && (line[len-1] == '\n' || line[len-1] == '\r'))
            line[--len] = '\0';
        if (len > 0) {
            comp_add_word(ce, line, src);
            added++;
        }
    }
    fclose(f);
    return added;
}

void comp_boost(CompEngine *ce, const char *word)
{
    for (int i = 0; i < ce->word_count; i++) {
        if (strcmp(ce->words[i].word, word) == 0) {
            ce->words[i].frequency += 5;
            return;
        }
    }
}
