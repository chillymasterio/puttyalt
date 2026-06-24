/* puttyalt_msgcatalog.c - Message catalog for translations.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#define MCT_MAX 128
typedef struct { char key[40]; char value[120]; } MctEntry;
typedef struct { MctEntry e[MCT_MAX]; int n; } MsgCatalog;
void mct_init(MsgCatalog *c) { if (c) c->n = 0; }
int mct_add(MsgCatalog *c, const char *key, const char *value) {
    if (!c || c->n >= MCT_MAX || !key || !value) return -1;
    strncpy(c->e[c->n].key, key, 39); c->e[c->n].key[39] = 0;
    strncpy(c->e[c->n].value, value, 119); c->e[c->n].value[119] = 0;
    c->n++;
    return 0;
}
/* Translate a key; returns the key itself if not found (gettext-style). */
const char *mct_get(const MsgCatalog *c, const char *key) {
    if (!c || !key) return key;
    for (int i = 0; i < c->n; i++) if (strcmp(c->e[i].key, key) == 0) return c->e[i].value;
    return key;
}
