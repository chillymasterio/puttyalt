#ifndef PUTTYALT_EXPORT2_H
#define PUTTYALT_EXPORT2_H

typedef enum {
    EXPORT_HTML = 0,
    EXPORT_JSON,
    EXPORT_PLAIN_TEXT,
    EXPORT_RTF
} ExportFormat;

typedef struct ExportOptions {
    ExportFormat format;
    int include_timestamps;
    int include_colors;
    int max_lines;         /* 0 = all */
    char title[128];
} ExportOptions;

int export_scrollback(const char *scrollback, int num_lines,
                      const ExportOptions *opts, const char *output_path);
int export_session_info(const char *host, int port, const char *user,
                        const ExportOptions *opts, const char *output_path);

#endif
