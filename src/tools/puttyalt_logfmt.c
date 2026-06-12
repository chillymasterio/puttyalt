/* puttyalt_logfmt.c - Structured logfmt formatter (key=value pairs). */
#include <string.h>
#include <stdio.h>
int logfmt_pair(char *out, int outlen, int pos, const char *key, const char *value) {
    if (!out || !key) return pos;
    int needs_quote = (value && (strchr(value, ' ') || strchr(value, '"') || strchr(value, '=')));
    if (pos > 0 && pos < outlen - 1) out[pos++] = ' ';
    if (needs_quote)
        pos += snprintf(out + pos, outlen - pos, "%s=\"%s\"", key, value);
    else
        pos += snprintf(out + pos, outlen - pos, "%s=%s", key, value ? value : "");
    return pos;
}
int logfmt_int(char *out, int outlen, int pos, const char *key, long value) {
    if (!out || !key) return pos;
    if (pos > 0 && pos < outlen - 1) out[pos++] = ' ';
    return pos + snprintf(out + pos, outlen - pos, "%s=%ld", key, value);
}
int logfmt_parse_value(const char *logline, const char *key, char *out, int outlen) {
    if (!logline || !key || !out) return -1;
    char pat[64];
    snprintf(pat, sizeof(pat), "%s=", key);
    const char *p = strstr(logline, pat);
    if (!p) return -1;
    p += strlen(pat);
    int o = 0;
    if (*p == '"') {
        p++;
        while (*p && *p != '"' && o < outlen - 1) out[o++] = *p++;
    } else {
        while (*p && *p != ' ' && o < outlen - 1) out[o++] = *p++;
    }
    out[o] = 0;
    return o;
}
