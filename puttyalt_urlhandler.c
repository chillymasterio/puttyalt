#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include "puttyalt_urlhandler.h"

#ifdef _WIN32
#include <windows.h>
#include <shellapi.h>
#endif

static const char *url_prefixes[] = {
    "https://", "http://", "ftp://", "ssh://", "file://", NULL
};

void url_init(URLHandler *uh)
{
    memset(uh, 0, sizeof(*uh));
    uh->highlight_enabled = 1;
    uh->underline_links = 1;
    uh->ctrl_click_open = 1;
}

static int is_url_char(char c)
{
    return isalnum((unsigned char)c) || c == '/' || c == '.' ||
           c == '-' || c == '_' || c == '~' || c == ':' || c == '@' ||
           c == '!' || c == '$' || c == '&' || c == '\'' || c == '(' ||
           c == ')' || c == '*' || c == '+' || c == ',' || c == ';' ||
           c == '=' || c == '%' || c == '?' || c == '#' || c == '[' ||
           c == ']';
}

int url_scan_line(URLHandler *uh, const char *line, int line_num)
{
    int found = 0;
    int len = strlen(line);

    for (int i = 0; i < len; i++) {
        for (int p = 0; url_prefixes[p]; p++) {
            int plen = strlen(url_prefixes[p]);
            if (i + plen < len &&
                strncmp(&line[i], url_prefixes[p], plen) == 0) {
                if (uh->num_matches >= URL_MAX_MATCHES) return found;

                int end = i + plen;
                while (end < len && is_url_char(line[end])) end++;

                /* Strip trailing punctuation */
                while (end > i + plen) {
                    char last = line[end - 1];
                    if (last == '.' || last == ',' || last == ')' ||
                        last == ';' || last == ':')
                        end--;
                    else break;
                }

                URLMatch *m = &uh->matches[uh->num_matches];
                m->start_line = line_num;
                m->start_col = i;
                m->end_line = line_num;
                m->end_col = end;
                int url_len = end - i;
                if (url_len >= URL_MAX_LEN) url_len = URL_MAX_LEN - 1;
                memcpy(m->url, &line[i], url_len);
                m->url[url_len] = '\0';
                uh->num_matches++;
                found++;
                i = end - 1;
                break;
            }
        }
    }
    return found;
}

int url_at_position(const URLHandler *uh, int line, int col,
                    char *url_out, int url_out_len)
{
    for (int i = 0; i < uh->num_matches; i++) {
        const URLMatch *m = &uh->matches[i];
        if (m->start_line == line && col >= m->start_col && col < m->end_col) {
            strncpy(url_out, m->url, url_out_len - 1);
            url_out[url_out_len - 1] = '\0';
            return 1;
        }
    }
    return 0;
}

void url_clear(URLHandler *uh)
{
    uh->num_matches = 0;
}

int url_open(const char *url)
{
#ifdef _WIN32
    return (int)(size_t)ShellExecuteA(NULL, "open", url, NULL, NULL, SW_SHOWNORMAL);
#elif defined(__APPLE__)
    char cmd[URL_MAX_LEN + 16];
    snprintf(cmd, sizeof(cmd), "open -- '%s'", url);
    return system(cmd);
#else
    char cmd[URL_MAX_LEN + 16];
    snprintf(cmd, sizeof(cmd), "xdg-open -- '%s'", url);
    return system(cmd);
#endif
}
