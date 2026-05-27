#include <string.h>
#include <stdio.h>
int diff_strings(const char *a, const char *b, char *out, int outlen) {
    int pos = 0, la = (int)strlen(a), lb = (int)strlen(b);
    int i = 0;
    while (i < la && i < lb) {
        if (a[i] != b[i]) {
            pos += snprintf(out + pos, outlen - pos, "@%d: '%c' -> '%c'\n", i, a[i], b[i]);
        }
        i++;
    }
    if (la > lb) pos += snprintf(out + pos, outlen - pos, "-%d chars at end\n", la - lb);
    else if (lb > la) pos += snprintf(out + pos, outlen - pos, "+%d chars at end\n", lb - la);
    return pos;
}
