#ifndef PUTTYALT_UNICODE_H
#define PUTTYALT_UNICODE_H

#define UNICODE_MAX_RECENT 32
#define UNICODE_MAX_FAVORITES 64

typedef struct {
    unsigned int codepoint;
    char name[64];
    char category[32];
} UnicodeEntry;

typedef struct {
    UnicodeEntry recent[UNICODE_MAX_RECENT];
    int recent_count;
    UnicodeEntry favorites[UNICODE_MAX_FAVORITES];
    int fav_count;
} UnicodeTable;

void unicode_init(UnicodeTable *ut);
int  unicode_add_recent(UnicodeTable *ut, unsigned int codepoint, const char *name);
int  unicode_add_favorite(UnicodeTable *ut, unsigned int codepoint, const char *name);
int  unicode_remove_favorite(UnicodeTable *ut, unsigned int codepoint);
int  unicode_to_utf8(unsigned int codepoint, char *out, int max);
int  unicode_from_utf8(const unsigned char *in, int len, unsigned int *out);

#endif
