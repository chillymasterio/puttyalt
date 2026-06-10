/* puttyalt_charcount.c - Character/word/line counter for terminal buffer. */
#include <string.h>
typedef struct { int chars; int words; int lines; int bytes; } CharCount;
void charcount_init(CharCount *c) { if(c) memset(c,0,sizeof(*c)); }
void charcount_process(CharCount *c, const char *text, int len) {
    if(!c||!text) return;
    int in_word=0;
    for (int i=0;i<len;i++) {
        unsigned char ch=text[i];
        c->bytes++;
        /* count UTF-8 chars (skip continuation bytes) */
        if ((ch & 0xC0) != 0x80) c->chars++;
        if (ch=='\n') c->lines++;
        if (ch==' '||ch=='\t'||ch=='\n') { in_word=0; }
        else if (!in_word) { c->words++; in_word=1; }
    }
}
int charcount_chars(const CharCount *c) { return c?c->chars:-1; }
int charcount_words(const CharCount *c) { return c?c->words:-1; }
int charcount_lines(const CharCount *c) { return c?c->lines:-1; }
