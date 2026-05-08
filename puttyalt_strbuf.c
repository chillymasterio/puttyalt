#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "puttyalt_strbuf.h"

void sb_init(StrBuf *sb, size_t initial_cap)
{
    sb->cap = initial_cap > 16 ? initial_cap : 16;
    sb->data = (char *)malloc(sb->cap);
    sb->len = 0;
    if (sb->data) sb->data[0] = '\0';
}

void sb_free(StrBuf *sb)
{
    free(sb->data);
    sb->data = NULL;
    sb->len = 0;
    sb->cap = 0;
}

static int sb_grow(StrBuf *sb, size_t need)
{
    if (sb->len + need + 1 <= sb->cap) return 0;
    size_t new_cap = sb->cap * 2;
    while (new_cap < sb->len + need + 1) new_cap *= 2;
    char *new_data = (char *)realloc(sb->data, new_cap);
    if (!new_data) return -1;
    sb->data = new_data;
    sb->cap = new_cap;
    return 0;
}

int sb_append(StrBuf *sb, const char *str)
{
    if (!str) return 0;
    size_t slen = strlen(str);
    if (sb_grow(sb, slen) < 0) return -1;
    memcpy(sb->data + sb->len, str, slen);
    sb->len += slen;
    sb->data[sb->len] = '\0';
    return 0;
}

int sb_append_n(StrBuf *sb, const char *str, size_t n)
{
    if (!str || n == 0) return 0;
    if (sb_grow(sb, n) < 0) return -1;
    memcpy(sb->data + sb->len, str, n);
    sb->len += n;
    sb->data[sb->len] = '\0';
    return 0;
}

int sb_appendf(StrBuf *sb, const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    char tmp[1024];
    int n = vsnprintf(tmp, sizeof(tmp), fmt, ap);
    va_end(ap);
    if (n < 0) return -1;
    return sb_append_n(sb, tmp, n);
}

int sb_append_char(StrBuf *sb, char c)
{
    if (sb_grow(sb, 1) < 0) return -1;
    sb->data[sb->len++] = c;
    sb->data[sb->len] = '\0';
    return 0;
}

void sb_clear(StrBuf *sb)
{
    sb->len = 0;
    if (sb->data) sb->data[0] = '\0';
}

const char *sb_str(const StrBuf *sb)
{
    return sb->data ? sb->data : "";
}

char *sb_detach(StrBuf *sb)
{
    char *result = sb->data;
    sb->data = NULL;
    sb->len = 0;
    sb->cap = 0;
    return result;
}
