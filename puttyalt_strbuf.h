#ifndef PUTTYALT_STRBUF_H
#define PUTTYALT_STRBUF_H

#include <stddef.h>

typedef struct StrBuf {
    char *data;
    size_t len;
    size_t cap;
} StrBuf;

void sb_init(StrBuf *sb, size_t initial_cap);
void sb_free(StrBuf *sb);
int  sb_append(StrBuf *sb, const char *str);
int  sb_append_n(StrBuf *sb, const char *str, size_t n);
int  sb_appendf(StrBuf *sb, const char *fmt, ...);
int  sb_append_char(StrBuf *sb, char c);
void sb_clear(StrBuf *sb);
const char *sb_str(const StrBuf *sb);
char *sb_detach(StrBuf *sb);

#endif
