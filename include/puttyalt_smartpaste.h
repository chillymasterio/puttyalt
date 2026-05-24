#ifndef PUTTYALT_SMARTPASTE_H
#define PUTTYALT_SMARTPASTE_H

#define PASTE_SAFE        0
#define PASTE_WARN_NEWLINE  1
#define PASTE_WARN_SUDO     2
#define PASTE_WARN_DANGEROUS 3
#define PASTE_WARN_MULTILINE 4

typedef struct {
    int enabled;
    int warn_multiline;
    int warn_sudo;
    int warn_rm;
    int warn_pipe_curl;
    int strip_trailing_newline;
    int confirm_threshold;  /* char count above which to confirm */
    int bracket_paste_mode; /* use \e[200~ bracketed paste */
    char last_warning[256];
} SmartPaste;

void  smartpaste_init(SmartPaste *sp);
int   smartpaste_analyze(SmartPaste *sp, const char *text, int len);
const char *smartpaste_warning_text(int level);
char *smartpaste_sanitize(SmartPaste *sp, const char *text, int len, int *out_len);
void  smartpaste_set_bracket_mode(SmartPaste *sp, int enabled);

#endif
