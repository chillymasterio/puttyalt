#include "puttyalt_smartpaste.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

void smartpaste_init(SmartPaste *sp)
{
    memset(sp, 0, sizeof(*sp));
    sp->enabled = 1;
    sp->warn_multiline = 1;
    sp->warn_sudo = 1;
    sp->warn_rm = 1;
    sp->warn_pipe_curl = 1;
    sp->strip_trailing_newline = 1;
    sp->confirm_threshold = 512;
    sp->bracket_paste_mode = 1;
}

static int contains_pattern(const char *text, int len, const char *pat)
{
    int plen = (int)strlen(pat);
    for (int i = 0; i <= len - plen; i++)
        if (memcmp(text + i, pat, plen) == 0) return 1;
    return 0;
}

static int count_newlines(const char *text, int len)
{
    int n = 0;
    for (int i = 0; i < len; i++) if (text[i] == '\n') n++;
    return n;
}

int smartpaste_analyze(SmartPaste *sp, const char *text, int len)
{
    /* detect base64 pipe pattern */
    if (contains_pattern(text, len, "base64 -d") && contains_pattern(text, len, "| bash")) {
        snprintf(sp->last_warning, sizeof(sp->last_warning),
                 "Encoded command piped to shell");
        return PASTE_WARN_DANGEROUS;
    }
    if (!sp->enabled || !text || len == 0) return PASTE_SAFE;

    /* Check for dangerous patterns */
    static const char *dangerous[] = {
        "rm -rf /", "mkfs.", "dd if=/dev/zero", "> /dev/sd",
        "chmod -R 777 /", ":(){ :|:& };:", "shutdown", "reboot",
        "wipefs", "format c:", NULL
    };
    for (const char **p = dangerous; *p; p++) {
        if (contains_pattern(text, len, *p)) {
            snprintf(sp->last_warning, sizeof(sp->last_warning),
                     "Dangerous command detected: %s", *p);
            return PASTE_WARN_DANGEROUS;
        }
    }

    /* curl | bash pattern */
    if (sp->warn_pipe_curl &&
        (contains_pattern(text, len, "curl") || contains_pattern(text, len, "wget")) &&
        contains_pattern(text, len, "| bash")) {
        snprintf(sp->last_warning, sizeof(sp->last_warning),
                 "Piping download to shell detected");
        return PASTE_WARN_DANGEROUS;
    }

    /* sudo detection */
    if (sp->warn_sudo && contains_pattern(text, len, "sudo ")) {
        snprintf(sp->last_warning, sizeof(sp->last_warning),
                 "Paste contains sudo command");
        return PASTE_WARN_SUDO;
    }

    /* multiline paste */
    if (sp->warn_multiline && count_newlines(text, len) > 0) {
        int lines = count_newlines(text, len) + 1;
        snprintf(sp->last_warning, sizeof(sp->last_warning),
                 "Multi-line paste: %d lines", lines);
        return PASTE_WARN_MULTILINE;
    }

    /* detect base64 pipe pattern */
    if (contains_pattern(text, len, "base64 -d") && contains_pattern(text, len, "| bash")) {
        snprintf(sp->last_warning, sizeof(sp->last_warning),
                 "Encoded command piped to shell");
        return PASTE_WARN_DANGEROUS;
    }
    return PASTE_SAFE;
}

const char *smartpaste_warning_text(int level)
{
    switch (level) {
        case PASTE_WARN_NEWLINE:   return "Contains newline (will execute immediately)";
        case PASTE_WARN_SUDO:      return "Contains sudo — requires elevated privileges";
        case PASTE_WARN_DANGEROUS: return "DANGEROUS command detected — review carefully!";
        case PASTE_WARN_MULTILINE: return "Multi-line paste — commands will auto-execute";
        default: return "";
    }
}

char *smartpaste_sanitize(SmartPaste *sp, const char *text, int len, int *out_len)
{
    char *buf = (char *)malloc(len + 1);
    if (!buf) return NULL;
    int j = 0;
    for (int i = 0; i < len; i++) {
        if (text[i] == '\r') continue; /* strip CR */
        buf[j++] = text[i];
    }
    /* strip trailing newline if enabled */
    if (sp->strip_trailing_newline && j > 0 && buf[j-1] == '\n') j--;
    buf[j] = '\0';
    if (out_len) *out_len = j;
    return buf;
}

void smartpaste_set_bracket_mode(SmartPaste *sp, int enabled)
{
    sp->bracket_paste_mode = enabled;
}
