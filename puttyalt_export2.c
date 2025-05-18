#include <stdio.h>
#include <string.h>
#include <time.h>
#include "puttyalt_export2.h"

static void write_html_header(FILE *fp, const char *title)
{
    fprintf(fp, "<!DOCTYPE html>\n<html><head><meta charset=\"utf-8\">\n");
    fprintf(fp, "<title>%s</title>\n", title);
    fprintf(fp, "<style>body{background:#1e1e1e;color:#d4d4d4;font-family:monospace;");
    fprintf(fp, "padding:20px;} pre{white-space:pre-wrap;}</style>\n");
    fprintf(fp, "</head><body>\n<h2>%s</h2>\n<pre>\n", title);
}

static void write_html_footer(FILE *fp)
{
    fprintf(fp, "</pre>\n</body></html>\n");
}

int export_scrollback(const char *scrollback, int num_lines,
                      const ExportOptions *opts, const char *output_path)
{
    FILE *fp = fopen(output_path, "w");
    if (!fp) return -1;

    const char *title = opts->title[0] ? opts->title : "Terminal Output";
    int max = opts->max_lines > 0 ? opts->max_lines : num_lines;
    if (max > num_lines) max = num_lines;

    switch (opts->format) {
    case EXPORT_HTML:
        write_html_header(fp, title);
        /* Write scrollback lines with HTML escaping */
        for (const char *p = scrollback; *p && max > 0; p++) {
            switch (*p) {
                case '<': fprintf(fp, "&lt;"); break;
                case '>': fprintf(fp, "&gt;"); break;
                case '&': fprintf(fp, "&amp;"); break;
                case '\n': fprintf(fp, "\n"); max--; break;
                default: fputc(*p, fp); break;
            }
        }
        write_html_footer(fp);
        break;

    case EXPORT_JSON: {
        fprintf(fp, "{\n  \"title\": \"%s\",\n", title);
        time_t now = time(NULL);
        char ts[64];
        strftime(ts, sizeof(ts), "%Y-%m-%dT%H:%M:%S", localtime(&now));
        fprintf(fp, "  \"exported_at\": \"%s\",\n", ts);
        fprintf(fp, "  \"lines\": [\n");
        int line_num = 0;
        const char *start = scrollback;
        for (const char *p = scrollback; ; p++) {
            if (*p == '\n' || *p == '\0') {
                if (line_num > 0) fprintf(fp, ",\n");
                fprintf(fp, "    \"");
                for (const char *c = start; c < p; c++) {
                    if (*c == '"') fprintf(fp, "\\\"");
                    else if (*c == '\\') fprintf(fp, "\\\\");
                    else fputc(*c, fp);
                }
                fprintf(fp, "\"");
                line_num++;
                if (*p == '\0' || line_num >= max) break;
                start = p + 1;
            }
        }
        fprintf(fp, "\n  ]\n}\n");
        break;
    }

    default: /* EXPORT_PLAIN_TEXT */
        fprintf(fp, "%.*s", (int)strlen(scrollback), scrollback);
        break;
    }

    fclose(fp);
    return 0;
}

int export_session_info(const char *host, int port, const char *user,
                        const ExportOptions *opts, const char *output_path)
{
    FILE *fp = fopen(output_path, "w");
    if (!fp) return -1;

    if (opts->format == EXPORT_JSON) {
        fprintf(fp, "{\"host\":\"%s\",\"port\":%d,\"user\":\"%s\"}\n",
                host, port, user ? user : "");
    } else {
        fprintf(fp, "Host: %s\nPort: %d\nUser: %s\n",
                host, port, user ? user : "");
    }
    fclose(fp);
    return 0;
}
