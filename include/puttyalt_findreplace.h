#ifndef PUTTYALT_FINDREPLACE_H
#define PUTTYALT_FINDREPLACE_H

#define FR_MAX_PATTERN   256
#define FR_MAX_MATCHES   1024

typedef struct FindMatch {
    int line;
    int col;
    int len;
} FindMatch;

typedef struct FindReplace {
    char pattern[FR_MAX_PATTERN];
    char replace[FR_MAX_PATTERN];
    FindMatch matches[FR_MAX_MATCHES];
    int num_matches;
    int current_match;
    int case_sensitive;
    int use_regex;
    int wrap_around;
    int highlight_all;
} FindReplace;

void fr_init(FindReplace *fr);
int  fr_search(FindReplace *fr, const char *text, int text_len,
               const char *pattern);
int  fr_next(FindReplace *fr);
int  fr_prev(FindReplace *fr);
void fr_clear(FindReplace *fr);
int  fr_get_current_line(const FindReplace *fr);
int  fr_get_current_col(const FindReplace *fr);

#endif
