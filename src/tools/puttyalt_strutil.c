/* puttyalt_strutil.c - String utilities (trim/split/replace/case). */
#include <string.h>
#include <stdio.h>
int strutil_trim(const char *in, char *out, int outlen) {
    if (!in || !out) return -1;
    const char *start = in;
    while (*start == ' ' || *start == '\t' || *start == '\n' || *start == '\r') start++;
    int len = (int)strlen(start);
    while (len > 0 && (start[len - 1] == ' ' || start[len - 1] == '\t' ||
                       start[len - 1] == '\n' || start[len - 1] == '\r'))
        len--;
    if (len >= outlen) len = outlen - 1;
    memcpy(out, start, len);
    out[len] = 0;
    return len;
}
int strutil_split(const char *in, char delim, char tokens[][64], int max_tokens) {
    if (!in || !tokens) return -1;
    int n = 0, ti = 0;
    for (const char *p = in; ; p++) {
        if (*p == delim || *p == 0) {
            if (n < max_tokens) {
                tokens[n][ti] = 0;
                n++;
                ti = 0;
            }
            if (*p == 0) break;
        } else if (n < max_tokens && ti < 63) {
            tokens[n][ti++] = *p;
        }
    }
    return n;
}
int strutil_replace(const char *in, const char *from, const char *to, char *out, int outlen) {
    if (!in || !from || !to || !out) return -1;
    int o = 0, fl = (int)strlen(from);
    const char *p = in;
    while (*p && o < outlen - 1) {
        if (strncmp(p, from, fl) == 0) {
            o += snprintf(out + o, outlen - o, "%s", to);
            p += fl;
        } else {
            out[o++] = *p++;
        }
    }
    out[o] = 0;
    return o;
}
int strutil_count(const char *in, const char *sub) {
    if (!in || !sub) return -1;
    int n = 0, sl = (int)strlen(sub);
    if (sl == 0) return 0;
    for (const char *p = in; (p = strstr(p, sub)); p += sl) n++;
    return n;
}
