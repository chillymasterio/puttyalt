/* puttyalt_tabstops.c - Tab-stop manager for PuttyAlt terminal. */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define TABSTOPS_MAX_COLS 1024
#define TABSTOPS_DEFAULT_EVERY 8

typedef struct {
    uint8_t set[TABSTOPS_MAX_COLS]; /* 1 = tab stop at this column */
    int cols;                       /* active terminal width */
    int every;                      /* default spacing for reset */
} tabstops_t;

void tabstops_reset(tabstops_t *t) /* default stop every N columns */
{
    int c;
    if (!t) return;
    memset(t->set, 0, sizeof(t->set));
    if (t->every <= 0) t->every = TABSTOPS_DEFAULT_EVERY;
    for (c = t->every; c < t->cols; c += t->every) t->set[c] = 1;
}

int tabstops_init(tabstops_t *t, int cols, int every)
{
    if (!t || cols < 0 || cols > TABSTOPS_MAX_COLS) return -1;
    t->cols = cols;
    t->every = (every > 0) ? every : TABSTOPS_DEFAULT_EVERY;
    tabstops_reset(t);
    return 0;
}

int tabstops_resize(tabstops_t *t, int cols)
{
    int c;
    if (!t || cols < 0 || cols > TABSTOPS_MAX_COLS) return -1;
    for (c = t->cols; c < cols; c++) /* seed defaults for new columns */
        t->set[c] = (t->every > 0 && c > 0 && c % t->every == 0) ? 1 : 0;
    for (c = cols; c < t->cols; c++) t->set[c] = 0;
    t->cols = cols;
    return 0;
}

int tabstops_set(tabstops_t *t, int col) /* HTS */
{
    if (!t || col < 0 || col >= t->cols) return -1;
    t->set[col] = 1;
    return 0;
}

int tabstops_clear(tabstops_t *t, int col) /* TBC clear-one */
{
    if (!t || col < 0 || col >= t->cols) return -1;
    t->set[col] = 0;
    return 0;
}

void tabstops_clear_all(tabstops_t *t) /* TBC clear-all */
{
    if (t) memset(t->set, 0, sizeof(t->set));
}

int tabstops_is_set(const tabstops_t *t, int col)
{
    if (!t || col < 0 || col >= t->cols) return 0;
    return t->set[col] ? 1 : 0;
}

int tabstops_next(const tabstops_t *t, int col, int right) /* clamp right margin */
{
    int c;
    if (!t) return -1;
    if (right < 0 || right >= t->cols) right = t->cols - 1;
    for (c = col + 1; c <= right; c++) if (t->set[c]) return c;
    return right;
}

int tabstops_prev(const tabstops_t *t, int col, int left) /* clamp left margin */
{
    int c;
    if (!t) return -1;
    if (left < 0) left = 0;
    for (c = col - 1; c >= left; c--) if (t->set[c]) return c;
    return left;
}

int tabstops_forward(const tabstops_t *t, int col, int count, int left, int right) /* CHT */
{
    int i, n = col;
    if (!t || count < 0) return -1;
    if (right < 0 || right >= t->cols) right = t->cols - 1;
    for (i = 0; i < count && n < right; i++) n = tabstops_next(t, n, right);
    if (n < left) n = left;
    return n;
}

int tabstops_backward(const tabstops_t *t, int col, int count, int left, int right) /* CBT */
{
    int i, n = col;
    if (!t || count < 0) return -1;
    if (left < 0) left = 0;
    for (i = 0; i < count && n > left; i++) n = tabstops_prev(t, n, left);
    if (right >= 0 && right < t->cols && n > right) n = right;
    return n;
}

int tabstops_count(const tabstops_t *t, char *buf, size_t bufsz)
{
    int c, n = 0;
    if (!t) return -1;
    for (c = 0; c < t->cols; c++) if (t->set[c]) n++;
    if (buf && bufsz > 0)
        snprintf(buf, bufsz, "tabstops=%d/%d every=%d", n, t->cols, t->every);
    return n;
}
