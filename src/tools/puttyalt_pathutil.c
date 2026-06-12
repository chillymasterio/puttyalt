/* puttyalt_pathutil.c - Path manipulation (join/basename/dirname/normalize). */
#include <string.h>
#include <stdio.h>
int pathutil_basename(const char *path, char *out, int outlen) {
    if (!path || !out) return -1;
    const char *slash = strrchr(path, '/');
    const char *bs = strrchr(path, '\\');
    if (bs && (!slash || bs > slash)) slash = bs;
    const char *base = slash ? slash + 1 : path;
    return snprintf(out, outlen, "%s", base);
}
int pathutil_dirname(const char *path, char *out, int outlen) {
    if (!path || !out) return -1;
    const char *slash = strrchr(path, '/');
    const char *bs = strrchr(path, '\\');
    if (bs && (!slash || bs > slash)) slash = bs;
    if (!slash) return snprintf(out, outlen, ".");
    int len = (int)(slash - path);
    if (len == 0) len = 1;
    if (len >= outlen) len = outlen - 1;
    memcpy(out, path, len);
    out[len] = 0;
    return len;
}
int pathutil_join(const char *a, const char *b, char *out, int outlen) {
    if (!a || !b || !out) return -1;
    int la = (int)strlen(a);
    int sep = (la > 0 && a[la - 1] != '/' && a[la - 1] != '\\');
    return snprintf(out, outlen, "%s%s%s", a, sep ? "/" : "", b);
}
int pathutil_extension(const char *path, char *out, int outlen) {
    if (!path || !out) return -1;
    const char *dot = strrchr(path, '.');
    const char *slash = strrchr(path, '/');
    if (!dot || (slash && dot < slash)) {
        out[0] = 0;
        return 0;
    }
    return snprintf(out, outlen, "%s", dot + 1);
}
