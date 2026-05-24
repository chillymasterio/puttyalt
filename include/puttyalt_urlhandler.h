#ifndef PUTTYALT_URLHANDLER_H
#define PUTTYALT_URLHANDLER_H

#define URL_MAX_LEN      2048
#define URL_MAX_MATCHES  32

typedef struct URLMatch {
    int start_col;
    int start_line;
    int end_col;
    int end_line;
    char url[URL_MAX_LEN];
} URLMatch;

typedef struct URLHandler {
    URLMatch matches[URL_MAX_MATCHES];
    int num_matches;
    int highlight_enabled;
    int underline_links;
    int ctrl_click_open;   /* require Ctrl+click to open */
} URLHandler;

void url_init(URLHandler *uh);
int  url_scan_line(URLHandler *uh, const char *line, int line_num);
int  url_at_position(const URLHandler *uh, int line, int col,
                     char *url_out, int url_out_len);
void url_clear(URLHandler *uh);
int  url_open(const char *url);  /* platform-specific launcher */

#endif
