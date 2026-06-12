/* puttyalt_charmap.c - Character mapping table (codepage/ACS translation). */
#include <stdint.h>
#include <string.h>
#define CM_MAX 128
typedef struct {
    uint8_t from[CM_MAX];
    uint32_t to[CM_MAX];
    int n;
} CharMap;
void charmap_init(CharMap *c) {
    if (c) memset(c, 0, sizeof(*c));
}
int charmap_add(CharMap *c, uint8_t from, uint32_t to) {
    if (!c || c->n >= CM_MAX) return -1;
    c->from[c->n] = from;
    c->to[c->n] = to;
    return c->n++;
}
uint32_t charmap_translate(const CharMap *c, uint8_t ch) {
    if (!c) return ch;
    for (int i = 0; i < c->n; i++)
        if (c->from[i] == ch) return c->to[i];
    return ch;
}
int charmap_has(const CharMap *c, uint8_t ch) {
    if (!c) return 0;
    for (int i = 0; i < c->n; i++)
        if (c->from[i] == ch) return 1;
    return 0;
}
int charmap_count(const CharMap *c) {
    return c ? c->n : -1;
}
