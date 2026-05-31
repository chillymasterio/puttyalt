/*
 * puttyalt_promptmark.c - Semantic prompt mark (OSC 133) parser for PuttyAlt.
 *
 * Parses FinalTerm/OSC 133 semantic prompt marks:
 *   A = prompt-start, B = input-start, C = output-start,
 *   D = command-end (optionally carrying ";<exit-code>").
 * Records the terminal row of each prompt-start in a rolling window and
 * supports prompt-to-prompt navigation and scrollback row adjustment.
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define PROMPTMARK_MAX 256

typedef enum {
    PROMPTMARK_NONE = 0,
    PROMPTMARK_PROMPT_START,  /* A */
    PROMPTMARK_INPUT_START,   /* B */
    PROMPTMARK_OUTPUT_START,  /* C */
    PROMPTMARK_COMMAND_END    /* D */
} promptmark_kind;

typedef struct {
    int row;               /* terminal/scrollback row of the prompt-start */
    int exit_code;         /* last command exit code, -1 if unknown */
    uint64_t now_ms;       /* timestamp when recorded */
} promptmark_entry;

typedef struct {
    promptmark_entry marks[PROMPTMARK_MAX];
    int count;             /* number of valid entries in marks[] */
    int head;              /* index of oldest entry (rolling window) */
    int pending_exit;      /* exit code parsed from last D, -1 if none */
} promptmark_state;

void promptmark_init(promptmark_state *st)
{
    if (!st)
        return;
    memset(st, 0, sizeof(*st));
    st->pending_exit = -1;
}

/* Parse the payload of an OSC 133 sequence (the bytes after "133;").
 * On a prompt-start (A) the current cursor row is recorded. Returns 0 on
 * success, -1 on error/ignored input. */
int promptmark_parse(promptmark_state *st, const char *payload,
                     int cursor_row, uint64_t now_ms)
{
    promptmark_kind kind;
    int idx, code;

    if (!st || !payload || cursor_row < 0)
        return -1;

    switch (payload[0]) {
        case 'A': kind = PROMPTMARK_PROMPT_START; break;
        case 'B': kind = PROMPTMARK_INPUT_START;  break;
        case 'C': kind = PROMPTMARK_OUTPUT_START; break;
        case 'D': kind = PROMPTMARK_COMMAND_END;  break;
        default:  return -1;
    }

    if (kind == PROMPTMARK_COMMAND_END) {
        code = -1;
        if (payload[1] == ';' && payload[2] != '\0') {
            char *end = NULL;
            long v = strtol(payload + 2, &end, 10);
            if (end != payload + 2 && v >= 0 && v <= 255)
                code = (int)v;
        }
        st->pending_exit = code;
        return 0;
    }

    if (kind != PROMPTMARK_PROMPT_START)
        return 0;

    if (st->count < PROMPTMARK_MAX) {
        idx = (st->head + st->count) % PROMPTMARK_MAX;
        st->count++;
    } else {
        idx = st->head;
        st->head = (st->head + 1) % PROMPTMARK_MAX;
    }
    st->marks[idx].row = cursor_row;
    st->marks[idx].exit_code = st->pending_exit;
    st->marks[idx].now_ms = now_ms;
    st->pending_exit = -1;
    return 0;
}

int promptmark_count(const promptmark_state *st)
{
    return st ? st->count : 0;
}

/* Row of the most recently recorded prompt, or -1 if none. */
int promptmark_last_prompt_row(const promptmark_state *st)
{
    int idx;
    if (!st || st->count <= 0)
        return -1;
    idx = (st->head + st->count - 1) % PROMPTMARK_MAX;
    return st->marks[idx].row;
}

/* Row of the nearest recorded prompt strictly above from_row, or -1. */
int promptmark_prev_prompt(const promptmark_state *st, int from_row)
{
    int i, best = -1;
    if (!st)
        return -1;
    for (i = 0; i < st->count; i++) {
        int idx = (st->head + i) % PROMPTMARK_MAX;
        int r = st->marks[idx].row;
        if (r < from_row && r > best)
            best = r;
    }
    return best;
}

/* Row of the nearest recorded prompt strictly below from_row, or -1. */
int promptmark_next_prompt(const promptmark_state *st, int from_row)
{
    int i, best = -1;
    if (!st)
        return -1;
    for (i = 0; i < st->count; i++) {
        int idx = (st->head + i) % PROMPTMARK_MAX;
        int r = st->marks[idx].row;
        if (r > from_row && (best == -1 || r < best))
            best = r;
    }
    return best;
}

/* Shift all recorded rows by delta (e.g. on scrollback). Marks scrolled
 * above row 0 are dropped. Returns number of remaining marks. */
int promptmark_scroll(promptmark_state *st, int delta)
{
    int i, w = 0;
    promptmark_entry tmp[PROMPTMARK_MAX];
    if (!st)
        return -1;
    for (i = 0; i < st->count; i++) {
        int idx = (st->head + i) % PROMPTMARK_MAX;
        int r = st->marks[idx].row + delta;
        if (r >= 0) {
            tmp[w] = st->marks[idx];
            tmp[w].row = r;
            w++;
        }
    }
    for (i = 0; i < w; i++)
        st->marks[i] = tmp[i];
    st->head = 0;
    st->count = w;
    return w;
}
