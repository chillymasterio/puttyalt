#include "puttyalt_urldetect.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void urldetect_init(URLDetector *ud)
{
    memset(ud, 0, sizeof(*ud));
    ud->enabled = 1;
    ud->underline_urls = 1;
    ud->click_opens = 1;
    ud->capacity = 64;
    ud->urls = calloc(ud->capacity, sizeof(DetectedURL));
}

void urldetect_destroy(URLDetector *ud) { free(ud->urls); ud->urls = NULL; }

static int is_url_char(char c) { return isalnum(c) || strchr("-._~:/?#[]@!$&'()*+,;=%", c) != NULL; }

static const char *prefixes[] = {"https://", "http://", "ftp://", "ssh://", "file://", NULL};

void urldetect_scan_line(URLDetector *ud, TermState *ts, int row)
{
    if (!ud->enabled || row < 0 || row >= ts->rows) return;
    char line[TERM_MAX_COLS + 1];
    for (int c = 0; c < ts->cols; c++) {
        TermCell *cell = term_cell_at(ts, row, c);
        line[c] = (cell && cell->ch >= 0x20 && cell->ch < 0x7F) ? (char)cell->ch : ' ';
    }
    line[ts->cols] = '\0';

    for (const char **pfx = prefixes; *pfx; pfx++) {
        int plen = (int)strlen(*pfx);
        char *pos = line;
        while ((pos = strstr(pos, *pfx)) != NULL) {
            int start = (int)(pos - line);
            int end = start + plen;
            while (end < ts->cols && is_url_char(line[end])) end++;
            /* Trim trailing punctuation */
            while (end > start + plen && strchr(".,;:!?)>\"'", line[end - 1])) end--;

            if (end - start > plen + 2 && ud->count < ud->capacity) {
                DetectedURL *u = &ud->urls[ud->count++];
                u->row = row; u->col_start = start; u->col_end = end - 1;
                memcpy(u->url, &line[start], end - start);
                u->url[end - start] = '\0';
            }
            pos = &line[end];
        }
    }
}

void urldetect_scan_all(URLDetector *ud, TermState *ts)
{
    urldetect_clear(ud);
    for (int r = 0; r < ts->rows; r++) urldetect_scan_line(ud, ts, r);
}

DetectedURL *urldetect_at(URLDetector *ud, int row, int col)
{
    for (int i = 0; i < ud->count; i++) {
        DetectedURL *u = &ud->urls[i];
        if (u->row == row && col >= u->col_start && col <= u->col_end) return u;
    }
    return NULL;
}

void urldetect_clear(URLDetector *ud) { ud->count = 0; }
