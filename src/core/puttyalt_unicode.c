#include "puttyalt_unicode.h"
#include <string.h>
#include <stdio.h>

void unicode_init(UnicodeTable *ut) { memset(ut, 0, sizeof(*ut)); }

int unicode_add_recent(UnicodeTable *ut, unsigned int cp, const char *name)
{
    /* Shift recent entries */
    if (ut->recent_count >= UNICODE_MAX_RECENT) {
        for (int i = 0; i < UNICODE_MAX_RECENT - 1; i++)
            ut->recent[i] = ut->recent[i + 1];
        ut->recent_count = UNICODE_MAX_RECENT - 1;
    }
    UnicodeEntry *e = &ut->recent[ut->recent_count++];
    e->codepoint = cp;
    if (name) snprintf(e->name, sizeof(e->name), "%s", name);
    return 0;
}

int unicode_add_favorite(UnicodeTable *ut, unsigned int cp, const char *name)
{
    if (ut->fav_count >= UNICODE_MAX_FAVORITES) return -1;
    for (int i = 0; i < ut->fav_count; i++)
        if (ut->favorites[i].codepoint == cp) return 0;
    UnicodeEntry *e = &ut->favorites[ut->fav_count++];
    e->codepoint = cp;
    if (name) snprintf(e->name, sizeof(e->name), "%s", name);
    return 0;
}

int unicode_remove_favorite(UnicodeTable *ut, unsigned int cp)
{
    for (int i = 0; i < ut->fav_count; i++) {
        if (ut->favorites[i].codepoint == cp) {
            for (int j = i; j < ut->fav_count - 1; j++)
                ut->favorites[j] = ut->favorites[j + 1];
            ut->fav_count--;
            return 0;
        }
    }
    return -1;
}

int unicode_to_utf8(unsigned int cp, char *out, int max)
{
    if (max < 5) return -1;
    if (cp < 0x80) {
        out[0] = (char)cp;
        out[1] = '\0';
        return 1;
    } else if (cp < 0x800) {
        out[0] = 0xC0 | (cp >> 6);
        out[1] = 0x80 | (cp & 0x3F);
        out[2] = '\0';
        return 2;
    } else if (cp < 0x10000) {
        out[0] = 0xE0 | (cp >> 12);
        out[1] = 0x80 | ((cp >> 6) & 0x3F);
        out[2] = 0x80 | (cp & 0x3F);
        out[3] = '\0';
        return 3;
    } else if (cp < 0x110000) {
        out[0] = 0xF0 | (cp >> 18);
        out[1] = 0x80 | ((cp >> 12) & 0x3F);
        out[2] = 0x80 | ((cp >> 6) & 0x3F);
        out[3] = 0x80 | (cp & 0x3F);
        out[4] = '\0';
        return 4;
    }
    return -1;
}

int unicode_from_utf8(const unsigned char *in, int len, unsigned int *out)
{
    if (len <= 0 || !in) return -1;
    if (in[0] < 0x80) { *out = in[0]; return 1; }
    if ((in[0] & 0xE0) == 0xC0 && len >= 2) {
        *out = ((in[0] & 0x1F) << 6) | (in[1] & 0x3F);
        return 2;
    }
    if ((in[0] & 0xF0) == 0xE0 && len >= 3) {
        *out = ((in[0] & 0x0F) << 12) | ((in[1] & 0x3F) << 6) | (in[2] & 0x3F);
        return 3;
    }
    if ((in[0] & 0xF8) == 0xF0 && len >= 4) {
        *out = ((in[0] & 0x07) << 18) | ((in[1] & 0x3F) << 12) |
               ((in[2] & 0x3F) << 6) | (in[3] & 0x3F);
        return 4;
    }
    return -1;
}
