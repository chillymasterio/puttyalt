/* puttyalt_csvwrite.c - CSV writer with proper escaping + header. */
#include <string.h>
#include <stdio.h>
int csvwrite_field(char *out, int outlen, int pos, const char *value, int is_first) {
    if (!out || !value) return pos;
    if (!is_first && pos < outlen - 1) out[pos++] = ',';
    int needs_quote = (strchr(value, ',') || strchr(value, '"') || strchr(value, '\n'));
    if (needs_quote) {
        if (pos < outlen - 1) out[pos++] = '"';
        for (const char *p = value; *p && pos < outlen - 2; p++) {
            if (*p == '"') out[pos++] = '"';
            out[pos++] = *p;
        }
        if (pos < outlen - 1) out[pos++] = '"';
    } else {
        pos += snprintf(out + pos, outlen - pos, "%s", value);
    }
    return pos;
}
int csvwrite_row(char *out, int outlen, const char **fields, int n) {
    if (!out || !fields) return -1;
    int pos = 0;
    for (int i = 0; i < n; i++) {
        pos = csvwrite_field(out, outlen, pos, fields[i], i == 0);
    }
    if (pos < outlen - 1) out[pos++] = '\n';
    out[pos] = 0;
    return pos;
}
